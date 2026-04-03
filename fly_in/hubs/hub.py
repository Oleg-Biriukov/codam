from pydantic import BaseModel, Field
from enum import Enum
from typing import Self, List, cast


class Zone(Enum):
    NORMAL = 'normal'
    BLOCKED = 'blocked'
    RESTRICTED = 'restricted'
    PRIORITY = 'priority'


class Color(Enum):
    NONE = None
    RED = 'red'
    YELLOW = 'yellow'
    BLUE = 'blue'
    GRAY = 'gray'
    GREEN = 'green'


# patteln ^ - all values;
# [some filter, if ^ we use this as exception]<size>(it could be +)$-end of str
class Hub(BaseModel):
    name: str = Field(pattern="^[a-zA-Z0-9]{2,15}$")
    pos: tuple[int, int]
    zone: Zone = Zone.NORMAL
    color: Color = Color.NONE
    max_drones: str = Field(pattern="^[0-9]{0,10}$", default='1')
    max_link_capacity: str = Field(pattern="^[0-9]{5,15}$", default='1')
    next: List[Self] = Field(repr=False, default=[])
    prev: List[Self] = Field(repr=False, default=[])
    _g: float = float('inf')
    parent: Self | None = None
    is_possible: bool = True

    def model_post_init(self, context):
        self.max_drones = cast(int, int(self.max_drones))
        self.max_link_capacity = cast(int, int(self.max_link_capacity))

    def __lt__(self, other) -> bool:
        return self._g < other._g


class Dron(BaseModel):
    id: int = Field(ge=0, le=10000)
    pos: Hub
    route: list[tuple[int, Hub]] = []

    def move_to(self, hub: Hub) -> bool:
        if (hub.max_drones > 0 and hub.zone != 'blocked' and
                hub.max_link_capacity > 0):
            self.pos.max_drones += 1
            hub.max_drones -= 1
            hub.max_link_capacity -= 1
            self.pos = hub
            return True
        return False
