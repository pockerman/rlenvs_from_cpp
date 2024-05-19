import gymnasium as gym
from typing import Any
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

from .gym_walk import WalkEnv

gym_walk_env_router = APIRouter(prefix="/gdrl/gym-walk-env", tags=["gym-walk-env"])

# the environment to create
env: WalkEnv | None = None
ENV_NAME = "GymWalkEnv"

# actions that the environment accepts
ACTIONS_SPACE = {0: "WEST", 1: "EAST"}


@gym_walk_env_router.get("/is-alive")
async def get_is_alive():
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@gym_walk_env_router.post("/make")
async def make(version: str = Body(default="v1"), n_states: int = Body(default=7, ge=2),
               p_stay: float = Body(default=0.0, ge=0.0, le=1.0),
               p_backward: float = Body(default=0.5, le=1.0, ge=0.0)) -> JSONResponse:
    global env
    if env is not None:
        env.close()

    try:
        env = WalkEnv(n_states=n_states, p_stay=p_stay, p_backward=p_backward)
    except Exception as e:
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@gym_walk_env_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        env.close()
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@gym_walk_env_router.post("/reset")
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


@gym_walk_env_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if action not in ACTIONS_SPACE:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

    if env is not None:
        observation, reward, terminated, truncated, info = env.step(action)

        step = TimeStep(observation=observation,
                        reward=reward,
                        step_type=TimeStepType.MID if terminated == False else TimeStepType.LAST,
                        info=info,
                        discount=1.0)

        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail=f"Environment {ENV_NAME} is not initialized. Have you called make()?")


@gym_walk_env_router.get("/dynamics")
async def get_dynamics(state: int, action: int = None) -> JSONResponse:
    global env

    if env is None:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Environment {ENV_NAME} does not exposes dynamics.")

    if state >= env.nS:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {state} should be in [0, {env.nS})")

    if action is not None:

        if action not in ACTIONS_SPACE:
            raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                                detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

        p = env.P[state][action]
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"p": p})

    p = env.P[state]
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"p": p})


@gym_walk_env_router.post("/sync")
async def sync(options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
