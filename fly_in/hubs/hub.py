from pydantic import BaseModel, Field, field_validator, PrivateAttr
from enum import Enum
from typing import Self, List
# webcolor
# color = name_to_rgb(clr)
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
    next: List[tuple[Self, int]] = Field(repr=False, default=[])
    _g: float = float('inf')
    parent: Self | None = None
    is_possible: bool = True

    @field_validator('max_drones', mode='before')
    @classmethod
    def auto_conversion(cls, v, info):
        if isinstance(v, str):
            if v == 'endless':
                return 10000000
            if info.field_name == 'max_drones':
                pattern = "^[0-9]{0,10}$"
            else:
                return v
            if not re.fullmatch(pattern, v):
                raise ValueError(f'{info.field_name} has invalid format.')
            return int(v)
        return v

    def add_next(self, item: tuple) -> bool:
        if item:
            h, c = item
            if re.fullmatch("^[0-9]{0,10}$", c):
                if h not in self.next:
                    self.next.append((h, int(c)))
                return False
        return True

    def __lt__(self, other) -> bool:
        return self._g < other._g


class Dron(BaseModel):
    id: int = Field(ge=0, le=10000)
    pos: Hub
    c_pos: tuple[int, int]
    route: list[tuple[int, Hub]] = []
    _to_wait: int = PrivateAttr(0)

    def move_to(self) -> bool:
        def search_hub(lst: list[tuple[Hub, int]]) -> int:
            i: int
            for i in range(len(lst)):
                h, _ = lst[i]
                if h == self.pos:
                    break
            return i

        if self._to_wait == 0:
            _, hub = self.route[0]
            index: int = search_hub(hub.next)
            nxt, m_x = hub.next[index]
            if (hub.max_drones > 0 and hub.zone != 'blocked' and
                    m_x > 0):
                _, hub = self.route.pop(0)
                if hub.zone.value == 'restricted':
                    self._to_wait += 1
                self.pos.max_drones += 1
                hub.next[index] = (nxt, m_x - 1)
                hub.max_drones -= 1
                self.pos = hub
                print(f'd{self.id} -> {self.pos.name}')
                return True
        else:
            self._to_wait -= 1
        return False


# class Sprite:
#     img: Surface

#     def __init__(self):
#         img = assets["drone"]
#     def draw() -> None:
#         pass


# assets = {}

# def load_assets(filepath):
#     for img in filepath:
#         assets[img] = pygame.image.load(img)