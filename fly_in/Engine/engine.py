from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List
from Engine.strategies import Strategy, Astar


class Engine(BaseModel):
    _data: DataConf = PrivateAttr()

    def configure(self, filename: str) -> None:
        ConfigCompiler.modify_path(filename)
        self._data = ConfigCompiler.get_values()
        for d in self._data['dron']:
            d.move_to(self._data['start_hub'])

    def make_turn(self, stg: Strategy) -> None:
        pass
