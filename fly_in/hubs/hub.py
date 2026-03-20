from pydantic import BaseModel, Field
from enum import Enum
from typing import Self, List


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
