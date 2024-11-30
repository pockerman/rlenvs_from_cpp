import enum
import copy
from typing import Generic, Optional, TypeVar
from pydantic import BaseModel, Field, Extra
import numpy as np

_Reward = TypeVar('_Reward')
_Discount = TypeVar('_Discount')
_Observation = TypeVar('_Observation')


class TimeStepType(enum.IntEnum):
    """Defines the status of a `TimeStep` within a sequence.

    """

    # Denotes the first `TimeStep` in a sequence.
    FIRST = 0

    # Denotes any `TimeStep` in a sequence that is not FIRST or LAST.
    MID = 1

    # Denotes the last `TimeStep` in a sequence.
    LAST = 2

    def first(self) -> bool:
        return self is TimeStepType.FIRST

    def mid(self) -> bool:
        return self is TimeStepType.MID

    def last(self) -> bool:
        return self is TimeStepType.LAST


class TimeStep(BaseModel, Generic[_Reward, _Discount, _Observation]):
    step_type: TimeStepType = Field(title="step_type")
    reward: Optional[_Reward] = Field(title="reward")
    discount: Optional[_Discount] = Field(title="discount")
    observation: _Observation = Field(title="observation")
    info: dict = Field(title="info")

    def first(self) -> bool:
        return self.step_type == TimeStepType.FIRST

    def mid(self) -> bool:
        return self.step_type == TimeStepType.MID

    def last(self) -> bool:
        return self.step_type == TimeStepType.LAST

    @property
    def done(self) -> bool:
        return self.last()

    # def dump_time_step(self) -> Dict:
    #     return {
    #         "step_type": self.step_type,
    #         "discount": self.discount,
    #         "observation": self.observation,
    #         "info": self.info
    #     }


class TimeStepV(BaseModel, Generic[_Reward, _Discount, _Observation]):
    step_types: list[TimeStepType] = Field(title='step_types')
    rewards: Optional[list[_Reward]] = Field(title='rewards')
    discounts: Optional[list[_Discount]] = Field(title='discounts')
    observations: Optional[list[_Observation]] = Field(title="observations")
    infos: list[dict] = Field(title="infos")
