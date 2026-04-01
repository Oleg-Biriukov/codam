from pydantic import BaseModel, Field, PrivateAttr
from enum import Enum
from typing import Self, List
from drons.dron import Dron


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
        self.max_drones = int(self.max_drones)
        self.max_link_capacity = int(self.max_link_capacity)

    def __lt__(self, other) -> bool:
        return self._g < other._g
