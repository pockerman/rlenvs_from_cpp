import gymnasium as gym
from typing import Any
from loguru import logger
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

black_jack_router = APIRouter(prefix="/gymnasium/black-jack-env", tags=["black-jack-env"])

# the environment to create
env = None
ENV_NAME = "Blackjack"

# actions that the environment accepts
ACTIONS_SPACE = {0: "STICK", 1: "HIT"}


@black_jack_router.get("/is-alive")
async def get_is_alive():
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@black_jack_router.post("/make")
async def make(version: str = Body(default="v1"), natural: bool = Body(default=False),
               sab: bool = Body(default=False)):
    global env
    if env is not None:
        env.close()

    try:
        env = gym.make(f"{ENV_NAME}-{version}", natural, sab)# max_episode_steps=max_episode_steps)
    except Exception as e:
        logger.error('Exception raised')
        logger.opt(exception=e).info("Logging exception traceback")
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@black_jack_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        env.close()
        env = None
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@black_jack_router.post("/reset")
async def reset(seed: int = Body(default=42), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global env

    if env is not None:
        observation, info = env.reset(seed=seed)
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


@black_jack_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if action not in ACTIONS_SPACE:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

    if env is not None:
        observation, reward, terminated, truncated, info = env.step(action)

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


@black_jack_router.get("/dynamics")
async def get_dynamics(stateId: int, actionId: int = None) -> JSONResponse:
    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail=f"Environment {ENV_NAME} does not exposes dynamics.")


@black_jack_router.post("/sync")
async def sync(options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
