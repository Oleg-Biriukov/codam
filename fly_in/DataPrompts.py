from typing import TypedDict
from enum import Enum


class Zone(Enum):
    NORMAL = 'normal'
    BLOCKED = 'blocked'
    RESTRICTED = 'restricted'
    PRIORITY = 'priority'


class HubError(Exception):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class ConfError(SyntaxError):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class ConnectionError(SyntaxError):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class GraphError(SyntaxError):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class Meta(TypedDict):
    zone: Zone
    color: str
    max_drones: str
    max_link_capacity: str
