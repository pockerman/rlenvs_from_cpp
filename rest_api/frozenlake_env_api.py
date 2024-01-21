import gymnasium as gym
from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

frozenlake_router = APIRouter(prefix="/frozen-lake-env", tags=["frozen-lake-env"])


# the environment to create
env = None


@frozenlake_router.get("/is-alive")
async def is_alive() -> JSONResponse:
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})

@frozenlake_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        env.close()
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": "Environment is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": "Environment has not been created"})

@frozenlake_router.post("/make")
async def make(version: str = Body(default='v1'),
               map_name: str = Body(default="4x4"),
               is_slippery: bool = Body(default=True)) -> JSONResponse:

    global  env
    if env is not None:
        env.close()

    try:
        env = gym.make(f"FrozenLake-{version}",
                       map_name=map_name, is_slippery=is_slippery)
    except Exception as e:
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})

@frozenlake_router.post("/reset")
async def reset(seed: int = Body(default=42)) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global env

    if env is not None:
        observation, info = env.reset(seed=seed)

        step = TimeStep(observation=observation, reward=0.0,
                        step_type=TimeStepType.FIRST, info=info,
                        discount=1.0)
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": "Environment FrozenLake is not initialized."
                                           " Have you called make()?"})

@frozenlake_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if env is not None:
        observation, reward, terminated, truncated, info = env.step(action)

        step = TimeStep(observation=observation, reward=reward,
                        step_type=TimeStepType.MID if terminated == False else TimeStepType.LAST,
                        info=info,
                        discount=1.0)

        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": "Environment FrozenLake is not initialized. Have you called make()?"})

@frozenlake_router.get("/dynamics")
async def get_dynamics(stateId: int, actionId: int = None) -> JSONResponse:
    global env

    if env is not None:

        if actionId is None or actionId < 0:
            state_dyns = env.P[stateId]
            return JSONResponse(status_code=status.HTTP_201_CREATED,
                                content={"dynamics": state_dyns})
        else:
            dynamics = env.P[stateId][actionId]
            return JSONResponse(status_code=status.HTTP_200_OK,
                                content={"dynamics": dynamics})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": "Environment FrozenLake is not initialized. Have you called make()?"})

