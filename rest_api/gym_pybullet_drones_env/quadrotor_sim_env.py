"""The class QuadrotorSimEnv controls the quadrotor
simulation

"""
from typing import Any
from time_step_response import TimeStep, TimeStepType
from .quadrotor import Quadrotor
from .wind_model import WindModel


class QuadrotorSimEnv:
    ENV_NAME = "QuadrotorSim"
    VERSIONS = ["v0"]

    @classmethod
    def make(cls, quadrotor: Quadrotor, wind_model: WindModel,
             dt: float = 0.0005,
             version: str = "v0") -> "QuadrotorSimEnv":
        if version == "v0":
            return QuadrotorSimEnv(quadrotor=quadrotor, wind_model=wind_model, dt=dt)
        raise ValueError(f"Invalid QuadrotorSimEnv. Valid versions are {QuadrotorSimEnv.VERSIONS}")

    def __init__(self, quadrotor: Quadrotor, wind_model: WindModel, dt: float):
        self._quadrotor = quadrotor
        self._wind_model = wind_model
        self._t: float = 0.0
        self._dt: float = dt

    @property
    def sim_time(self) -> float:
        return self._t

    def reset(self, seed: int) -> TimeStep:
        pass

    def step(self, action: Any) -> TimeStep:
        self._quadrotor.update(cmd=action, wind=self._wind_model,
                               t=self._t, dt=self._dt)

        self._t += self._dt

    def close(self) -> None:
        self._quadrotor = None
        self._wind_model = None
