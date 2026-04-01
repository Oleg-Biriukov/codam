from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr
from typing import ClassVar, Generator, List, TypedDict
from hubs.hub import Hub
from drons.dron import Dron
import heapq
import functools


class DataConf(TypedDict):
    dron: List[Dron]
    start_hub: Hub | None
    end_hub: Hub | None
    hubs: List[Hub | None]


class Strategy(ABC, BaseModel):
    total_turns: ClassVar[int]

    @abstractmethod
    def perform_turn(self, dron: Dron, data: DataConf) -> None:
        pass

    @classmethod
    def set_to_null(cls) -> None:
        cls.total_turns = 0

    @classmethod
    def get_turns(cls) -> int:
        return cls.total_turns


class Astar(Strategy):
    @functools.lru_cache()
    def perform_turn(self, dron: Dron, data: DataConf) -> None:
        pos: Hub
        open_list: List[Hub] = []
        close_list: List[Hub] = []
        dron.pos._g = 0
        heapq.heappush(open_list, dron.pos)
        while open_list:
            pos = heapq.heappop(open_list)
            if pos == data['end_hub']:
                return
            close_list.append(pos)
            for n in pos.next:
                if (n in close_list or pos.max_drones < 1 or
                        n.is_possible is False):
                    continue
                if n.zone.value == 'restricted':
                    new_g = pos._g + 2
                else:
                    new_g = pos._g + 1
                if n not in open_list and n._g > new_g:
                    n.parent = pos
                    n._g = new_g
                    heapq.heappush(open_list, n)
