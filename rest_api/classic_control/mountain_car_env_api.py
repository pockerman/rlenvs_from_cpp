import gymnasium as gym
from typing import Union
from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from pydantic import BaseModel
from time_step_response import TimeStep, TimeStepType

mountain_car_router = APIRouter(prefix="/mountain-car-env", tags=["mountain-car-env"])

# the environment to create
env = None
ENV_NAME = "CartPole"

# actions that the environment accepts
ACTIONS_SPACE = {0: "Accelerate to the left", 1: "Don't accelerate", 2: "Accelerate to the right"}


class MountainCarParams(BaseModel):
    pass


@mountain_car_router.get("/action-space")
async def get_action_space() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"action_space": ACTIONS_SPACE})


@mountain_car_router.get("/is-alive")
async def get_is_alive() -> JSONResponse:
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@mountain_car_router.post("/make")
async def make(version: str = Body(default="v1"),
               params: MountainCarParams = Body(default=None)) -> JSONResponse:
    global env
    if env is not None:
        env.close()

    try:
        env = gym.make(f"{ENV_NAME}-{version}")
    except Exception as e:
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@mountain_car_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        env.close()
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@mountain_car_router.post("/reset")
async def reset(seed: int = Body(default=42)) -> JSONResponse:
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


@mountain_car_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if action not in ACTIONS_SPACE:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

    if env is not None:
        observation, reward, terminated, truncated, info = env.step(action)

        step = TimeStep(observation=observation,
                        reward=reward,
                        step_type=TimeStepType.MID if not terminated else TimeStepType.LAST,
                        info=info,
                        discount=1.0)

        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail=f"Environment {ENV_NAME} is not initialized. Have you called make()?")
