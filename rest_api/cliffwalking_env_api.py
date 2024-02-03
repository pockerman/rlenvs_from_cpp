import gymnasium as gym
from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStep, TimeStepType

cliffwalking_router = APIRouter(prefix="/cliffwalking-env", tags=["cliffwalking-env"])

# the environment to create
env = None
ENV_NAME = "CliffWalking"


@cliffwalking_router.get("/is-alive")
async def get_is_alive():
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@cliffwalking_router.post("/make")
async def make(version: str = Body(default="v0")):
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


@cliffwalking_router.post("/close")
async def close() -> JSONResponse:
    global env

    if env is not None:
        env.close()
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@cliffwalking_router.post("/reset")
async def reset(seed: int = Body(default=42)) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global env

    if env is not None:
        print(env.reset(seed=seed))
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


@cliffwalking_router.post("/step")
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


@cliffwalking_router.get("/dynamics")
async def get_dynamics(stateId: int, actionId: int = None) -> JSONResponse:
    global env

    if env is not None:

        if actionId is None or actionId < 0:
            state_dyns = env.P[stateId]

            print(state_dyns)
            state_dyns = {}
            return JSONResponse(status_code=status.HTTP_201_CREATED,
                                content={"dynamics": state_dyns})
        else:
            dynamics = env.P[stateId][actionId]
            new_dynamics = [(float(item[0]), int(item[1]), int(item[2]), item[3]) for item in dynamics]

            return JSONResponse(status_code=status.HTTP_200_OK,
                                content={"dynamics": new_dynamics})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized. "
                                           f"Have you called make()?"})
