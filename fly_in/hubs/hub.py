from pydantic import BaseModel, Field, field_validator
from enum import Enum
from typing import Self, List
import re


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
    ORANGE = 'orange'
    CYAN = 'cyan'
    PURPLE = 'purple'
    BROWN = 'brown'
    LIME = 'lime'
    MEGENTA = 'magenta'
    GOLD = 'gold'
    BLACK = 'black'
    MAROON = 'maroon'
    DARKRED = 'darkred'
    VIOLET = 'violet'
    CRIMSON = 'crimson'
    RAINBOW = 'rainbow'


# patteln ^ - all values;
# [some filter, if ^ we use this as exception]<size>(it could be +)$-end of str
class Hub(BaseModel):
    name: str = Field(pattern="^[a-zA-Z0-9_]{2,30}$")
    pos: tuple[int, int]
    zone: Zone = Zone.NORMAL
    color: Color = Color.NONE
    max_drones: int = Field(default=1)
    max_link_capacity: int = Field(default=1)
    next: List[Self] = Field(repr=False, default=[])
    prev: List[Self] = Field(repr=False, default=[])
    _g: float = float('inf')
    parent: Self | None = None
    is_possible: bool = True

    @field_validator('max_drones', 'max_link_capacity', mode='before')
    @classmethod
    def auto_conversion(cls, v, info):
        if isinstance(v, str):
            if v == 'endless':
                return 10000000
            if info.field_name == 'max_drones':
                pattern = "^[0-9]{0,10}$"
            elif info.field_name == 'max_link_capacity':
                pattern = "^[0-9]{0,10}$"
            else:
                return v
            if not re.fullmatch(pattern, v):
                raise ValueError(f'{info.field_name} has invalid format.')
            return int(v)
        return v

    def __lt__(self, other) -> bool:
        return self._g < other._g


class Dron(BaseModel):
    id: int = Field(ge=0, le=10000)
    pos: Hub
    route: list[tuple[int, Hub]] = []

    def move_to(self) -> bool:
        _, hub = self.route[::-1][0]
        if (hub.max_drones > 0 and hub.zone != 'blocked'):
            _, hub = self.route.pop(0)
            self.pos.max_drones += 1
            hub.max_drones -= 1
            # hub.max_link_capacity -= 1
            self.pos = hub
            return True
        return False
