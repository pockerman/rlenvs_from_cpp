import sys
from loguru import logger
from typing import Any
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

from .gym_walk import WalkEnv

gym_walk_env_router = APIRouter(prefix="/gdrl/gym-walk-env", tags=["gym-walk-env"])

# the environment to create
# the environments to create
envs = {
    0: None
}

ENV_NAME = "GymWalkEnv"

# actions that the environment accepts
ACTIONS_SPACE = {0: "WEST", 1: "EAST"}


@gym_walk_env_router.get("/is-alive")
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


@gym_walk_env_router.post("/close")
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


@gym_walk_env_router.post("/make")
async def make(version: str = Body(default="v1"),
               cidx: int = Body(...),
               options: dict[str, Any] = Body(default={"n_states": 7,
                                                       "p_stay": 0.0,
                                                       "p_backward": 0.5}),
              ) -> JSONResponse:
    global envs

    # n_states: int = Body(default=7, ge=2),
    # p_stay: float = Body(default=0.0, ge=0.0, le=1.0),
    # p_backward: float = Body(default=0.5, le=1.0, ge=0.0)

    n_states = options.get("n_states", 7)
    p_stay = options.get("p_stay", 0.0)
    p_backward = options.get("p_backward", 0.5)

    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            envs[cidx].close()

        try:
            env = WalkEnv(n_states=n_states, p_stay=p_stay, p_backward=p_backward)
            envs[cidx] = env
        except Exception as e:
            exception = sys.exc_info()
            logger.opt(exception=exception).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))
    else:
        try:
            env = WalkEnv(n_states=n_states, p_stay=p_stay, p_backward=p_backward)
            envs[cidx] = env
        except Exception as e:
            exception = sys.exc_info()
            logger.opt(exception=exception).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))

    logger.info(f'Created environment  {ENV_NAME} and index {cidx}')

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@gym_walk_env_router.post("/reset")
async def reset(seed: int = Body(default=42), cidx: int = Body(...),
                options: dict[str, Any] = Body(default={})) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            observation, info = envs[cidx].reset(seed=seed)

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


@gym_walk_env_router.post("/step")
async def step(action: int = Body(...), cidx: int = Body(...)) -> JSONResponse:

    if action not in ACTIONS_SPACE:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            observation, reward, terminated, truncated, info = envs[cidx].step(action)

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


@gym_walk_env_router.get("/dynamics")
async def get_dynamics(cidx: int, state: int, action: int = None) -> JSONResponse:
    global envs

    env = None
    if cidx in envs:
        env = envs[cidx]

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
async def sync(cidx: int = Body(...), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
