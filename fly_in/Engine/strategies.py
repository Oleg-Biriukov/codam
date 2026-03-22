from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr
from typing import ClassVar, Generator, List
from fly_in import Dron, DataConf


class Strategy(ABC, BaseModel):
    total_turns: ClassVar[int]

    @abstractmethod
    def perform_turn(self, drons: List[Dron]) -> Generator[None, List[Dron],
                                                           None]:
        pass

    @classmethod
    def set_to_null(cls) -> None:
        cls.total_turns = 0

    @classmethod
    def get_turns(cls) -> int:
        return cls.total_turns


class Astar(Strategy):
    # class Tools:
    #     @staticmethod
    #     def euclidean_distance(data: DataConf) -> float:
    #         import math as m
    #         return m.sqrt((x2-x1)**2 + (y2-y1)**2)

    g: float = PrivateAttr(float('inf'))
    h: float = PrivateAttr(0.0)

    @property
    def f(self) -> float:
        return self.g + self.h

    def perform_turn(self, drons: List[Dron]) -> Generator[None, List[Dron],
                                                           None]:
        dron: Dron = drons[0]
        
        yield
