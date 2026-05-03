from typing import TypedDict, List
from hubs.hub import Zone, Dron, Hub

global LINES
LINES: int = 0


class HubError(Exception):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class ConfError(SyntaxError):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class ConnectionError(SyntaxError):
    def __init__(self, message: str) -> None:
        super().__init__(message)


class DataConf(TypedDict):
    dron: List[Dron]
    start_hub: Hub | None
    end_hub: Hub | None
    hubs: List[Hub | None]


class Meta(TypedDict):
    zone: Zone
    color: str
    max_drones: str
    max_link_capacity: str
