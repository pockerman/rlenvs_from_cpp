import gymnasium as gym
from typing import Any
from loguru import logger
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

cart_pole_router = APIRouter(prefix="/gymnasium/cart-pole-env", tags=["cart-pole-env"])

ENV_NAME = "CartPole"

# the environments to create
envs = {
    0: None
}

# actions that the environment accepts
ACTIONS_SPACE = {0: "Push cart to the left", 1: "Push cart to the right"}


@cart_pole_router.get("/action-space")
async def get_action_space() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"action_space": ACTIONS_SPACE})


@cart_pole_router.get("/is-alive")
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


@cart_pole_router.post("/close")
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


@cart_pole_router.post("/make")
async def make(version: str = Body(default="v1"),
               cidx: int = Body(...),
               options: dict[str, Any] = Body(default={})) -> JSONResponse:
    global envs
    env_type = f"{ENV_NAME}-{version}"
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            envs[cidx].close()

        try:
            env = gym.make(env_type,)
            envs[cidx] = env
        except Exception as e:
            logger.error('An exception was raised')
            logger.opt(exception=e).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))
    else:
        try:
            env = gym.make(env_type)
            envs[cidx] = env
        except Exception as e:
            logger.error('An exception was raised')
            logger.opt(exception=e).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))

    logger.info(f'Created environment {ENV_NAME} and index {cidx}')
    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@cart_pole_router.post("/reset")
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
            observation, info = envs[cidx].reset(seed=seed)
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


@cart_pole_router.post("/step")
async def step(action: int = Body(...), cidx: int = Body(...)) -> JSONResponse:

    if action not in ACTIONS_SPACE:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

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


@cart_pole_router.post("/sync")
async def sync(cidx: int = Body(...), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
