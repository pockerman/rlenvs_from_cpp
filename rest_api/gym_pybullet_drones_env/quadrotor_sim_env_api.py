"""Script demonstrating the joint use of simulation and control.

The simulation is run by a `CtrlAviary` environment.
The control is given by the PID implementation in `DSLPIDControl`.

Example
-------
In a terminal, run as:

    $ python pid.py

Notes
-----
The drones move, at different altitudes, along cicular trajectories
in the X-Y plane, around point (0, -.3).

"""
from typing import List, Any
from fastapi import APIRouter, Depends, Body, status
from fastapi.responses import JSONResponse
from fastapi import HTTPException
import os
import time
import argparse
from datetime import datetime
import pdb
import math
import random
import numpy as np
import pybullet as p

from gym_pybullet_drones.utils.enums import DroneModel, Physics
from gym_pybullet_drones.envs.CtrlAviary import CtrlAviary
from gym_pybullet_drones.control.DSLPIDControl import DSLPIDControl
from gym_pybullet_drones.utils.Logger import Logger
from gym_pybullet_drones.utils.utils import sync, str2bool

from time_step_response import TimeStep, TimeStepType

quadrotor_sim_router = APIRouter(prefix="/gym-pybullet-drones/quadcopter-sim-env", tags=["quadcopter-sim-env"])

ENV_NAME: str = "gym-pybullet-drones/quadcopter-sim-env"
NUM_DRONES: int = 1
TARGET_POS: List | None = None
INIT_XYZS: List | None = None
INIT_RPYS: List | None = None
PERIOD: int = 10
NUM_WP: int
SIM_COUNTER: int = 0

wp_counters: List | None = None

env: CtrlAviary | None = None
quadcopter_model: DroneModel | None = None
physics: Physics | None = None
ctrl: List | None = None
action: List | None = None
logger: Logger | None = None


@quadrotor_sim_router.get("/action-space")
async def get_action_space() -> JSONResponse:
    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail="No action_space for gym-pybullet-drones/quadcopter-sim-env")


@quadrotor_sim_router.get("/is-alive")
async def get_is_alive() -> JSONResponse:
    global env

    if env is None:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": False})
    else:
        return JSONResponse(status_code=status.HTTP_200_OK,
                            content={"result": True})


@quadrotor_sim_router.post("/make")
async def make(version: str = Body(default="v0"),
               drone_model_type: str = Body(default="cf2x"),
               physics_type: str = Body(default="pyb"),
               num_drones: int = Body(default=1, le=10, ge=1),
               neighbourhood_radius: int = Body(default=10),
               simulation_freq_hz: int = Body(default=240),
               control_freq_hz: int = Body(default=48),
               show_gui: bool = Body(default=False),
               record_video: bool = Body(default=False),
               use_default_obstacles: bool = Body(default=False),
               user_debug_gui: bool = Body(default=False),
               output_folder: str = Body(default="results"),
               H_STEP: float = Body(default=0.05),
               H: float = Body(default=0.1),
               R: float = Body(default=0.3),
               ) -> JSONResponse:
    if num_drones <= 0:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail="num_drones cannot be <=0")

    global env
    global quadcopter_model
    global physics
    global action
    global ctrl
    global wp_counters
    global logger

    global NUM_DRONES
    global INIT_XYZS
    global INIT_RPYS
    global PERIOD
    global NUM_WP
    global TARGET_POS

    if env is not None:
        env.close()

    try:

        NUM_DRONES = num_drones
        quadcopter_model = DroneModel(drone_model_type)
        physics = Physics(physics_type)

        INIT_XYZS = np.array([[R * np.cos((i / 6) * 2 * np.pi + np.pi / 2),
                               R * np.sin((i / 6) * 2 * np.pi + np.pi / 2) - R, H + i * H_STEP] for i in
                              range(num_drones)])
        INIT_RPYS = np.array([[0, 0, i * (np.pi / 2) / num_drones] for i in range(num_drones)])

        NUM_WP = control_freq_hz * PERIOD
        TARGET_POS = np.zeros((NUM_WP, 3))

        for i in range(NUM_WP):
            TARGET_POS[i, :] = R * np.cos((i / NUM_WP) * (2 * np.pi) + np.pi / 2) + INIT_XYZS[0, 0], R * np.sin(
                (i / NUM_WP) * (2 * np.pi) + np.pi / 2) - R + INIT_XYZS[0, 1], 0

        wp_counters = np.array([int((i * NUM_WP / 6) % NUM_WP) for i in range(num_drones)])

        env = CtrlAviary(drone_model=quadcopter_model,
                         num_drones=num_drones,
                         initial_xyzs=INIT_XYZS,
                         initial_rpys=INIT_RPYS,
                         physics=physics,
                         neighbourhood_radius=neighbourhood_radius,
                         pyb_freq=simulation_freq_hz,
                         ctrl_freq=control_freq_hz,
                         gui=show_gui,
                         record=record_video,
                         obstacles=use_default_obstacles,
                         user_debug_gui=user_debug_gui
                         )

        logger = Logger(logging_freq_hz=control_freq_hz,
                        num_drones=num_drones,
                        output_folder=output_folder,
                        colab=False
                        )

        if quadcopter_model in [DroneModel.CF2X, DroneModel.CF2P]:
            ctrl = [DSLPIDControl(drone_model=quadcopter_model) for i in range(num_drones)]

        action = np.zeros((num_drones, 4))

    except Exception as e:
        raise HTTPException(status_code=status.HTTP_500_INTERNAL_SERVER_ERROR,
                            detail=str(e))

    return JSONResponse(status_code=status.HTTP_201_CREATED,
                        content={"result": True})


@quadrotor_sim_router.post("/close")
async def close() -> JSONResponse:
    global env
    global quadcopter_model
    global physics
    global logger
    global SIM_COUNTER

    if env is not None:
        env.close()
        logger.save()
        quadcopter_model = None
        physics = None
        logger = None
        env = None
        SIM_COUNTER = 0
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"message": f"Environment {ENV_NAME} is closed"})

    return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST,
                        content={"message": f"Environment {ENV_NAME} has not been created"})


@quadrotor_sim_router.post("/reset")
async def reset(seed: int = Body(default=42), options: dict[str, Any] = Body(default={})) -> JSONResponse:
    """Reset the environment

    :return:
    """

    global env
    global NUM_DRONES

    if env is not None:
        observation, info = env.reset(seed=seed)
        observation_arr = []

        for idx in range(NUM_DRONES):
            drone_data = observation[idx]

            drone_data = [float(val) for val in drone_data]
            observation_arr.append(drone_data)

        step = TimeStep(observation=observation_arr,
                        reward=0.0,
                        step_type=TimeStepType.FIRST,
                        info=info,
                        discount=1.0)
        return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                            content={"time_step": step.model_dump()})

    raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                        detail={"message": f"Environment {ENV_NAME} is not initialized."
                                           " Have you called make()?"})


@quadrotor_sim_router.post("/step")
async def step(action: int = Body(...)) -> JSONResponse:
    global env

    if env is None:
        raise HTTPException(status_code=status.HTTP_400_BAD_REQUEST,
                            detail=f"Environment {ENV_NAME} is not initialized. Have you called make()?")

    global ctrl
    global wp_counters
    global logger
    global NUM_DRONES
    global INIT_XYZS
    global INIT_RPYS
    global TARGET_POS
    global NUM_WP
    global SIM_COUNTER

    # Step the simulation
    observation, reward, terminated, truncated, info = env.step(action)

    step = TimeStep(observation=observation,
                    reward=reward,
                    step_type=TimeStepType.MID if not terminated else TimeStepType.LAST,
                    info=info,
                    discount=1.0)

    # Compute control for the current way point
    for j in range(NUM_DRONES):
        action[j, :], _, _ = ctrl[j].computeControlFromState(control_timestep=env.CTRL_TIMESTEP,
                                                             state=observation[j],
                                                             target_pos=np.hstack([TARGET_POS[wp_counters[j], 0:2],
                                                                                   INIT_XYZS[j, 2]]),

                                                             target_rpy=INIT_RPYS[j, :]
                                                             )

    # Go to the next way point and loop
    for j in range(NUM_DRONES):
        wp_counters[j] = wp_counters[j] + 1 if wp_counters[j] < (NUM_WP - 1) else 0

    # Log the simulation
    for j in range(NUM_DRONES):
        logger.log(drone=j,
                   timestamp=SIM_COUNTER / env.CTRL_FREQ,
                   state=observation[j],
                   control=np.hstack([TARGET_POS[wp_counters[j], 0:2],
                                      INIT_XYZS[j, 2],
                                      INIT_RPYS[j, :], np.zeros(6)]))

    SIM_COUNTER += 1

    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"time_step": step.model_dump()})


@quadrotor_sim_router.post("/sync")
async def sync(options: dict[str, Any] = Body(default={})) -> JSONResponse:
    return JSONResponse(status_code=status.HTTP_202_ACCEPTED,
                        content={"message": "OK"})
