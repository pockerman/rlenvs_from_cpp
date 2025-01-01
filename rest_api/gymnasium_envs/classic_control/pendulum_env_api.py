"""API for the Pendulum environment.
For general information see: https://gymnasium.farama.org/environments/classic_control/pendulum/

The action is a ndarray with shape (1,) representing the torque applied to free end of the pendulum.

"""
from loguru import logger
import gymnasium as gym
from typing import Any
import sys
from fastapi import APIRouter, Body, status
from fastapi import HTTPException
from fastapi.responses import JSONResponse
from time_step_response import TimeStep, TimeStepType

pendulum_router = APIRouter(prefix="/gymnasium/pendulum-env",
                            tags=["pendulum-env"])

ENV_NAME = "Pendulum"

# the environments to create
envs = {
    0: None
}

# actions that the environment accepts
ACTIONS_SPACE = [1, ]


@pendulum_router.get("/action-space")
async def get_action_space() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"action_space": ACTIONS_SPACE})


@pendulum_router.get("/is-alive")
async def get_is_alive(cidx: int) -> JSONResponse:
    global envs

    if cidx in envs:
        env = envs[cidx]

        if env is None:
            return JSONResponse(status_code=status.HTTP_200_OK,
                                content={"result": False})
        else:
            return JSONResponse(status_code=status.HTTP_200_OK,
                                content={"result": True})
    else:
        return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                            content={"message": f"Environment {ENV_NAME} and index {cidx} has not been created"})


@pendulum_router.post("/close")
async def close(cidx: int) -> JSONResponse:
    global envs
    if cidx in envs:
        env = envs[cidx]
        if env is not None:
            envs[cidx].close()
            envs[cidx] = None
            logger.info(f'Closed environment {ENV_NAME}  and index {cidx}')
            return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                content={"message": f"Environment {ENV_NAME} and index {cidx} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} and index {cidx} has not been created"})


@pendulum_router.post("/make")
async def make(version: str = Body(default="v1"),
               cidx: int = Body(...),
               options: dict[str, Any] = Body(default={"g": 10.0, "max_episode_steps": 200})) -> JSONResponse:
    global envs
    env_type = f"{ENV_NAME}-{version}"
    g = options.get("g", 10.0)
    max_episode_steps = options.get("max_episode_steps", 200)
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            envs[cidx].close()

        try:
            env = gym.make(env_type, g=g, max_episode_steps=max_episode_steps)
            envs[cidx] = env
            return JSONResponse(status_code=status.HTTP_201_CREATED,
                                content={"result": True})
        except Exception as e:
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))

    else:
        try:
            env = gym.make(id=env_type,
                           g=g, max_episode_steps=max_episode_steps)
            envs[cidx] = env
        except Exception as e:
            exception = sys.exc_info()
            logger.opt(exception=exception).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))

    logger.info(f'Created environment {ENV_NAME} and index {cidx}')
    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@pendulum_router.post("/reset")
async def reset(seed: int = Body(default=42),
                cidx: int = Body(...),
                options: dict[str, Any] = Body(default={})) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            logger.info(f"Resetting environment {ENV_NAME}")
            observation, info = envs[cidx].reset(seed=seed, options=options)
            observation = [float(val) for val in observation]
            step = TimeStep(observation=observation,
                            reward=0.0,
                            step_type=TimeStepType.FIRST,
                            info=info,
                            discount=1.0)
            logger.info(f'Reset environment {ENV_NAME}  and index {cidx}')
            return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized."
                                           " Have you called make()?"})


@pendulum_router.post("/step")
async def step(action: float = Body(...), cidx: int = Body(...)) -> JSONResponse:
    if not (action >= -2.0 and action <= 2.0):
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {ACTIONS_SPACE}")

    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            observation, reward, terminated, truncated, info = envs[cidx].step(action)
            observation = [float(val) for val in observation]

            step_type = TimeStepType.MID
            if terminated or truncated:
                step_type = TimeStepType.LAST

            step = TimeStep(observation=observation,
                            reward=reward,
                            step_type=step_type,
                            info=info,
                            discount=1.0)
            logger.info(f'Step in environment {ENV_NAME} and index {cidx}')
            return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail=f"Environment {ENV_NAME} is not initialized. Have you called make()?")


@pendulum_router.post("/sync")
async def sync(cidx: int = Body(...), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
