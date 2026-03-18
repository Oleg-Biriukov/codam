from pydantic import BaseModel, Field, PositiveInt
from enum import Enum
from typing import Self


class Zone(Enum):
    NORMAL = 'normal'
    BLOCKED = 'blocked'
    RESTRICTED = 'restricted'
    PRIORITY = 'priority'


class Color(Enum):
    NONE = None


# patteln ^ - all values;
# [some filter, if ^ we use this as exception]<size>(it could be +)$-end of str
class Hub(BaseModel):
    name: str = Field(pattern="^[a-zA-Z0-9]{5,15}$")
    pos: tuple[PositiveInt, PositiveInt]
    zone: Zone = Zone.NORMAL
    color: Color = Color.NONE
    max_drones: str = Field(pattern="^[0-9]{5,15}$", default='1')
    max_link_capacity: str = Field(pattern="^[0-9]{5,15}$", default='1')
    next: Self | None = None
    prev: Self | None = None
