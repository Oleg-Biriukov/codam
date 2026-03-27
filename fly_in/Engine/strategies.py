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
    g: float = PrivateAttr(0.0)
    h: float = PrivateAttr(0.0)

    def perform_turn(self, data: DataConf) -> Generator[Hub, List[Dron],
                                                        None]:
        dron: Dron = data['dron'][0]
        pos: Hub
        open_list: List[tuple[float, Hub]] = []
        close_list: set[Hub] = set()
        heapq.heappush(open_list, (dron._pos.f, dron._pos))
        while open_list:
            _, pos = heapq.heappop(open_list)
            if pos in close_list:
                continue
            close_list.add(pos)
            if pos == data['start_hub']:
                pos.g = 0
                pos.h = Astar.euclidean_distance(pos, data['end_hub'])
            for n in pos.next:
                if n == data['end_hub']:
                    return
                if n.zone == 'restricted':
                    n.g = pos.g + 2
                else:
                    n.g = pos.g + 1
                n.h = Astar.euclidean_distance(n, data['end_hub'])
                n.parent = pos
                heapq.heappush(open_list, (n.f, n))

        yield

    @staticmethod
    def euclidean_distance(src: Hub, dst: Hub) -> float:
        import math as m
        x1, y1 = src.pos
        x2, y2 = dst.pos
        return m.sqrt((x2-x1)**2 + (y2-y1)**2)
