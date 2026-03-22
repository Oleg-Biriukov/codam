from drons.dron import Dron
from typing import TypedDict, List
from hubs.hub import Hub


class DataConf(TypedDict):
    dron: List[Dron]
    start_hub: Hub | None
    end_hub: Hub | None
    hubs: List[Hub | None]


__all__ = ['HubError', 'ConfError', 'Dron', 'Hub']
