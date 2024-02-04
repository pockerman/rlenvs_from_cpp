import gymnasium as gym
from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

taxi_router = APIRouter(prefix="/taxi-env", tags=["taxi-env"])

# the environment to create
env = None
ENV_NAME = "Taxi"


@taxi_router.get("/is-alive")
async def get_is_alive():
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@taxi_router.post("/make")
async def make(version: str = Body(default="v3")):
    global env
    if env is not None:
        env.close()

    try:
        env = gym.make(f"Taxi-{version}")
    except Exception as e:
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@taxi_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        env.close()
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@taxi_router.post("/reset")
async def reset(seed: int = Body(default=42)) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global env

    if env is not None:
        print(env.reset(seed=seed))
        observation, info = env.reset(seed=seed)

        action_mask = info['action_mask']
        step = TimeStep(observation=observation,
                        reward=0.0,
                        step_type=TimeStepType.FIRST,
                        info={'action_mask': [int(i) for i in action_mask], 'prob': float(info['prob'])},
                        discount=1.0)
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized."
                                           " Have you called make()?"})


@taxi_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if env is not None:
        observation, reward, terminated, truncated, info = env.step(action)

        action_mask = info['action_mask']
        step = TimeStep(observation=observation,
                        reward=reward,
                        step_type=TimeStepType.MID if terminated == False else TimeStepType.LAST,
                        info={'action_mask': [int(i) for i in action_mask], 'prob': float(info['prob'])},
                        discount=1.0)

        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized. "
                                           f"Have you called make()?"})


@taxi_router.get("/dynamics")
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
                        detail={"message": f"Environment {ENV_NAME} is not initialized. "
                                           f"Have you called make()?"})
