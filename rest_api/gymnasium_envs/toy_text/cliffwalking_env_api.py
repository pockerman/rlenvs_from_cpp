import gymnasium as gym
from pydantic import BaseModel, Field
from typing import Any
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from loguru import logger
from time_step_response import TimeStep, TimeStepType

cliff_walking_router = APIRouter(prefix="/gymnasium/cliff-walking-env", tags=["cliff-walking-env"])

ENV_NAME = "CliffWalking"

# the environment to create
envs = {
    0: None
}


@cliff_walking_router.get("/is-alive")
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
                            content={"message": f"Environment {cidx} has not been created"})


@cliff_walking_router.post("/close")
async def close(cidx: int) -> JSONResponse:
    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            envs[cidx].close()
            envs[cidx] = None
            logger.info(f'Closed environment {ENV_NAME} and index {cidx}')
            return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@cliff_walking_router.post("/make")
async def make(version: str = Body(default="v1"),
               cidx: int = Body(...),
               max_episode_steps: int = Body(default=500)) -> JSONResponse:
    global envs

    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            envs[cidx].close()

        try:
            env = gym.make(id=f"{ENV_NAME}-{version}",
                           max_episode_steps=max_episode_steps)
            envs[cidx] = env
        except Exception as e:
            logger.error('Exception raised')
            logger.opt(exception=e).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))

    logger.info(f'Created environment  {ENV_NAME} and index {cidx}')
    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@cliff_walking_router.post("/reset")
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


@cliff_walking_router.post("/step")
async def step(action: int = Body(...), cidx: int = Body(...)) -> JSONResponse:
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
                                info={'prob': float(info['prob'])},
                                discount=1.0)

                logger.info(f'Step in environment {ENV_NAME} and index {cidx}')
                return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                    content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized. "
                                           f"Have you called make()?"})


@cliff_walking_router.get("/dynamics")
async def get_dynamics(cidx: int, stateId: int, actionId: int = None) -> JSONResponse:
    global envs

    if cidx in envs:
        env = envs[cidx]
        if env is not None:

            if actionId is None or actionId < 0:
                state_dyns = envs[cidx].P[stateId]

                print(state_dyns)
                state_dyns = {}
                return JSONResponse(status_code=status.HTTP_201_CREATED,
                                    content={"dynamics": state_dyns})
            else:
                dynamics = envs[cidx].P[stateId][actionId]
                new_dynamics = [(float(item[0]), int(item[1]), int(item[2]), item[3]) for item in dynamics]

                return JSONResponse(status_code=status.HTTP_200_OK,
                                    content={"dynamics": new_dynamics})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized. "
                                           f"Have you called make()?"})


@cliff_walking_router.post("/sync")
async def sync(cidx: int, options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
