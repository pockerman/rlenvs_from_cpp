"""The Acrobot environment from gymnasium:
for more information check: https://gymnasium.farama.org/environments/classic_control/acrobot/

"""
import gymnasium as gym
from typing import Any
from loguru import logger
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

acrobot_router = APIRouter(prefix="/gymnasium/acrobot-env", tags=["Acrobot-env API"])

# the environment to create
env = None
ENV_NAME = "Acrobot"

# actions that the environment accepts
ACTIONS_SPACE = {0: "apply -1 torque to the actuated joint",
                 1: "apply 0 torque to the actuated joint",
                 2: "apply 1 torque to the actuated joint"}


@acrobot_router.get("/action-space")
async def get_action_space() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"action_space": ACTIONS_SPACE})


@acrobot_router.get("/is-alive")
async def get_is_alive() -> JSONResponse:
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@acrobot_router.post("/make")
async def make(version: str = Body(default="v1"),  max_episode_steps: int = Body(default=500)) -> JSONResponse:
    global env
    if env is not None:
        env.close()

    try:
        logger.info(f"Creating environment {ENV_NAME}-{version}")
        env = gym.make(f"{ENV_NAME}-{version}")
    except Exception as e:
        logger.error('An exception was raised')
        logger.opt(exception=e).info("Logging exception traceback")
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@acrobot_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        logger.info(f"Closing environment {ENV_NAME}")
        env.close()
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@acrobot_router.post("/reset")
async def reset(seed: int = Body(default=42), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global env

    if env is not None:
        logger.info(f"Resetting environment {ENV_NAME} with seed={seed} and options={options}")
        if len(options) != 0:
            observation, info = env.reset(seed=seed, options=options)
        else:
            observation, info = env.reset(seed=seed)
        observation = [float(val) for val in observation]
        step = TimeStep(observation=observation,
                        reward=0.0,
                        step_type=TimeStepType.FIRST,
                        info=info,
                        discount=1.0)
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized."
                                           " Have you called make()?"})


@acrobot_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if action not in ACTIONS_SPACE:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

    if env is not None:
        logger.info(f"Stepping in environment {ENV_NAME} with action={action}")
        observation, reward, terminated, truncated, info = env.step(action)
        observation = [float(val) for val in observation]

        step_type = TimeStepType.MID
        if terminated or truncated:
            step_type = TimeStepType.LAST

        step = TimeStep(observation=observation,
                        reward=reward,
                        step_type=step_type,
                        info=info,
                        discount=1.0)

        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail=f"Environment {ENV_NAME} is not initialized. Have you called make()?")


@acrobot_router.post("/sync")
async def sync(options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
