from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List
from Engine.strategies import Strategy
from hubs.hub import Hub


class Engine(BaseModel):
    _data: DataConf = PrivateAttr()
    stg: Strategy

    def configure(self, filename: str) -> None:
        ConfigCompiler.modify_path(filename)
        self._data = ConfigCompiler.get_values()
        for d in self._data['dron']:
            d.move_to(self._data['start_hub'])

    def make_turn(self) -> None:
        self.stg.perform_turn(self._data)
        route: List = []
        pos: Hub = self._data['end_hub']
        while pos != self._data['start_hub']:
            route.append(pos.name)
            pos = pos.parent
        print('->'.join(route[::-1]))
