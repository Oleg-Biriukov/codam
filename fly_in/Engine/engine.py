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
#         for h in self._data['hubs']:
#             print(f'''Name: {h.name}
# Position: {h.pos}
# Zone: {h.zone}
# Color: {h.color}
# Maxium drones: {h.max_drones}
# Maxium links capacity: {h.max_link_capacity}''')
#             for n in h.next:
#                 print(f'''Next of {h.name}`s\n\tName: {n.name}
# \tPosition: {n.pos}
# \tZone: {n.zone}
# \tColor: {n.color}
# \tMaxium drones: {n.max_drones}
# \tMaxium links capacity: {n.max_link_capacity}
# ''')
#             for n in h.prev:
#                 print(f'''Prev of {h.name}`s\n\tName: {n.name}
# \tPosition: {n.pos}
# \tZone: {n.zone}
# \tColor: {n.color}
# \tMaxium drones: {n.max_drones}
# \tMaxium links capacity: {n.max_link_capacity}
# ''')

    def make_turn(self) -> None:
        self.stg.perform_turn(self._data)
        route: List = []
        pos: Hub = self._data['end_hub']
        while pos != self._data['start_hub']:
            route.append((pos.name, pos._g))
            pos = pos.parent
        for n in route[::-1]: print(n, '-> ', end='')
        print(self._data['end_hub']._g)
