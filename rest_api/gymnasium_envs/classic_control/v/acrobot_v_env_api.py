"""The vector Acrobot environment from gymnasium:
for more information check: https://gymnasium.farama.org/environments/classic_control/acrobot/
and https://gymnasium.farama.org/api/vector/sync_vector_env/

"""
import sys
import gymnasium as gym
from typing import Any, List
from loguru import logger
from fastapi import APIRouter, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
from time_step_response import TimeStepType, TimeStepV

acrobot_v_router = APIRouter(prefix="/gymnasium/acrobot-env/v", tags=["Acrobot Vector env API"])


NUM_COPIES = 0
ENV_NAME = "Acrobot"
VECTORIZATION_MODE = 'sync'

# the environments to create
envs = {
    0: None
}


# actions that the environment accepts
ACTIONS_SPACE = {0: "apply -1 torque to the actuated joint",
                 1: "apply 0 torque to the actuated joint",
                 2: "apply 1 torque to the actuated joint"}


def numpy_arr_to_arr(observations: List[List]) -> list[list[float]]:
    observations_ar = []
    for obs in observations:
        observations_ar.append([float(val) for val in obs])
    return observations_ar


@acrobot_v_router.get("/action-space")
async def get_action_space() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"action_space": ACTIONS_SPACE})


@acrobot_v_router.get("/vec-mode")
async def get_vector_mode() -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_200_OK,
                        content={"vector_mode": VECTORIZATION_MODE})


@acrobot_v_router.get("/is-alive")
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


@acrobot_v_router.post("/close")
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


@acrobot_v_router.post("/make")
async def make(version: str = Body(default="v1"),
               cidx: int = Body(...),
               options: dict[str, Any] = Body(default={"num_envs": 2})) -> JSONResponse:
    global env
    global NUM_COPIES
    global envs

    env_type = f"{ENV_NAME}-{version}"
    num_envs = options.get("num_envs", 2)
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            env.close()

        try:

            env = gym.make_vec(id=env_type,
                               num_envs=num_envs, vectorization_mode=VECTORIZATION_MODE)
            NUM_COPIES = num_envs
            envs[cidx] = env
        except Exception as e:
            logger.opt(exception=e).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))
    else:
        try:
            gym.make_vec(id=env_type,
                         num_envs=num_envs, vectorization_mode=VECTORIZATION_MODE)
            NUM_COPIES = num_envs
            envs[cidx] = env
        except Exception as e:
            exception = sys.exc_info()
            logger.opt(exception=exception).info("Logging exception traceback")
            raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                                detail=str(e))

    logger.info(f'Created environment  {ENV_NAME} and index {cidx}')
    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True, "vector_mode": VECTORIZATION_MODE,
                                 'num_envs': num_envs})


@acrobot_v_router.post("/reset")
async def reset(seed: int = Body(default=42), cidx: int = Body(...),
                options: dict[str, Any] = Body(default={})) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            logger.info(f"Resetting environment {ENV_NAME} with seed={seed} and options={options}")
            if len(options) != 0:
                observations, infos = envs[cidx].reset(seed=seed, options=options)
            else:
                # observations is an array with num_envs arrays
                observations, infos = envs[cidx].reset(seed=seed)

            observations_ar = numpy_arr_to_arr(observations)
            step = TimeStepV(observations=observations_ar,
                             rewards=[0.0] * NUM_COPIES,
                             step_types=[TimeStepType.FIRST] * NUM_COPIES,
                             infos=[],
                             discounts=[1.0] * NUM_COPIES)
            logger.info(f'Reset environment {ENV_NAME}  and index {cidx}')
            return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized."
                                           " Have you called make()?"})


@acrobot_v_router.post("/step")
async def step(action: dict[str, list[int]] = Body(title='actions'), cidx: int = Body(...)) -> JSONResponse:

    global NUM_COPIES

    actions = action['actions']
    if len(actions) != NUM_COPIES:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"actions length should be {NUM_COPIES} ")

    for action in actions:
        if action not in ACTIONS_SPACE:
            raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                                detail=f"Action {action} not in {list(ACTIONS_SPACE.keys())}")

    global envs
    if cidx in envs:
        env = envs[cidx]

        if env is not None:
            logger.info(f"Stepping in environment {ENV_NAME} with actions={actions}")
            observations, rewards, terminates, truncates, infos = env.step(actions)

            observations_ar = numpy_arr_to_arr(observations)

            step_types = [TimeStepType.MID] * NUM_COPIES

            # if we truncate or terminate
            # set the environment step type to finished
            for i, tr in enumerate(truncates):
                if tr:
                    step_types[i] = TimeStepType.LAST

                if terminates[i]:
                    step_types[i] = TimeStepType.LAST

            step = TimeStepV(observations=observations_ar,
                             rewards=rewards,
                             step_types=step_types,
                             infos=[],
                             discounts=[1.0] * NUM_COPIES)

            logger.info(f'Step in environment {ENV_NAME} and index {cidx}')
            return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                                content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail=f"Environment {ENV_NAME} is not initialized. Have you called make()?")


@acrobot_v_router.post("/sync")
async def sync(cidx: int = Body(...), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
