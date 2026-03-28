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
        heapq.heapify(open_list)
        heapq.heappush(open_list, (dron._pos._f, dron._pos))
        while open_list:
            _, pos = heapq.heappop(open_list)
            if pos == data['end_hub']:
                return
            if pos in close_list:
                continue
            if pos == data['start_hub']:
                pos._g = 0
                pos._h = Astar.euclidean_distance(pos, data['end_hub'])
            close_list.append(pos)
            for n in pos.next:
                if n == data['end_hub']:
                    n.parent = pos
                    return
                if n.zone == 'restricted':
                    n._g = pos._g + 2
                else:
                    n._g = pos._g + 1
                new_f: float = Astar.euclidean_distance(n, data['end_hub']) + n._g
                if n not in open_list or n._f >= new_f:
                    n.parent = pos
                    n._h = Astar.euclidean_distance(n, data['end_hub'])
                    heapq.heappush(open_list, (n._f, n))
                    break

    @staticmethod
    def euclidean_distance(src: Hub, dst: Hub) -> float:
        import math as m
        x1, y1 = src.pos
        x2, y2 = dst.pos
        return m.sqrt((x2-x1)**2 + (y2-y1)**2)
