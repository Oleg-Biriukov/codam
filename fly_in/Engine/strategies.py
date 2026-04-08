from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr
from typing import ClassVar, Generator, List, TypedDict
from hubs.hub import Hub, Dron
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
    def perform_turn(self, dron: Dron, data: DataConf, turns: int) -> None:
        pass

    @classmethod
    def set_to_null(cls) -> None:
        cls.total_turns = 0

    @classmethod
    def get_turns(cls) -> int:
        return cls.total_turns


class Astar(Strategy):
    # @functools.lru_cache()

    def perform_turn(self, dron: Dron, data: DataConf, turns: int) -> None:
        def is_valid_paths(dron: Dron, next: Hub) -> bool:
            for turn, hub in dron.route:
                if (hub == data['end_hub']
                        or hub == data['start_hub']):
                    continue
                for d in data['dron']:
                    if d == dron:
                        continue
                    for t, h in d.route:
                        if t == turn and h == hub and next in d.pos.next:
                            return False
            return True

        def get_value(n: Hub) -> int:
            t_hub: list[int] = [t for d in data['dron']
                                for t, h in d.route
                                if h == n]
            if t_hub:
                return min(t_hub)
            return 0

        pos: Hub
        open_list: List[Hub] = []
        close_list: List[Hub] = []
        dron.pos._g = turns
        heapq.heappush(open_list, dron.pos)
        while open_list:
            pos = heapq.heappop(open_list)
            if pos == data['end_hub']:
                return
            close_list.append(pos)
            for n in pos.next:
                if (n in close_list or
                        not is_valid_paths(dron, n)):
                    continue
                if n.zone.value == 'restricted':
                    new_g = pos._g + 2 + get_value(n)
                else:
                    new_g = pos._g + 1 + get_value(n)
                if n not in open_list and n._g > new_g:
                    n.parent = pos
                    n._g = new_g
                    heapq.heappush(open_list, n)
