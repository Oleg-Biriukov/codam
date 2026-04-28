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
    def perform_turn(self, dron: Dron, data: DataConf, turns: int) -> None:
        # @functools.lru_cache(maxsize=1000)
        def is_valid_paths(dron: Dron, next: Hub) -> bool:
            count_drons = 0
            for turn, hub in dron.route:
                if (hub == data['end_hub']
                        or hub == data['start_hub']):
                    continue
                for d in data['dron']:
                    if d == dron:
                        continue
                    for t, h in d.route:
                        if t == turn and h == hub and next in d.pos.next:
                            count_drons += 1
            if next.max_drones < count_drons and next in dron.pos.next:
                return False
            return True

        pos: Hub
        priority: int
        new_g: int
        open_list: List[Hub] = []
        close_list: List[Hub] = []
        dron.pos._g = turns
        heapq.heappush(open_list, (1, dron.pos))
        while open_list:
            _, pos = heapq.heappop(open_list)
            if pos == data['end_hub']:
                return
            close_list.append(pos)
            for n, c in pos.next:
                if (n in close_list or
                        not is_valid_paths(dron, n) or
                        (n.max_drones < 1 and (n, c) in dron.pos.next) or
                        (c < 1 and
                         (n, c) in dron.pos.next)):
                    continue
                priority = 1
                if n.zone.value == 'restricted':
                    new_g = pos._g + 2
                else:
                    if n.zone.value == 'priority':
                        priority = 0
                    new_g = pos._g + 1
                if n not in open_list and n._g > new_g:
                    n.parent = pos
                    n._g = new_g
                    heapq.heappush(open_list, (priority, n))
