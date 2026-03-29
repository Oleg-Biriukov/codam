from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr
from typing import ClassVar, Generator, List, TypedDict
from hubs.hub import Hub
from drons.dron import Dron
import heapq


class DataConf(TypedDict):
    dron: List[Dron]
    start_hub: Hub | None
    end_hub: Hub | None
    hubs: List[Hub | None]


class Strategy(ABC, BaseModel):
    total_turns: ClassVar[int]

    @abstractmethod
    def perform_turn(self, data: DataConf) -> None:
        pass

    @classmethod
    def set_to_null(cls) -> None:
        cls.total_turns = 0

    @classmethod
    def get_turns(cls) -> int:
        return cls.total_turns


class Astar(Strategy):
    def perform_turn(self, data: DataConf) -> None:
        dron: Dron = data['dron'][0]
        pos: Hub
        open_list: List[tuple[float, Hub]] = []
        close_list: List[Hub] = []
        dron._pos._g = 0
        heapq.heappush(open_list, dron._pos)
        while open_list:
            pos = heapq.heappop(open_list)
            if pos == data['end_hub']:
                return
            close_list.append(pos)
            for n in pos.next:
                if n in close_list:
                    continue
                if n.zone.value == 'restricted':
                    new_g = pos._g + 2
                else:
                    new_g = pos._g + 1
                if n not in open_list and n._g > new_g:
                    n.parent = pos
                    n._g = new_g
                    heapq.heappush(open_list, n)
