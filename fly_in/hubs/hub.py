from abc import ABC, abstractmethod
from pydantic import BaseModel, Field, PositiveInt
from enum import Enum
from typing import Self
from connections import Connection


class Zone(Enum):
    NORMAL = 'normal'
    BLOCKED = 'blocked'
    RESTRICTED = 'restricted'
    PRIORITY = 'priority'


class Color(Enum):
    NONE = None


# patteln ^ - all values;
# [some filter, if ^ we use this as exception]<size>(it could be +)$-end of str
class Hub(Connection, BaseModel):
    name: str = Field(pattern="^[^-]{5,15}$")
    pos: tuple[PositiveInt, PositiveInt] = Field()
    zone: Zone = Zone.NORMAL
    color: Color = Color.NONE
    max_drones: int = Field(ge=1, le=50, default=1)
    max_link_capacity: int = Field(gt=0, le=50, default=1)
    next: Self | None = None
    prev: Self | None = None
