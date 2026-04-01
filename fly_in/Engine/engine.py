from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List
from Engine.strategies import Strategy
from hubs.hub import Hub
from drons.dron import Dron


class Engine(BaseModel):
    _data: DataConf = PrivateAttr()
    stg: Strategy

    def configure(self, filename: str) -> None:
        ConfigCompiler.modify_path(filename)
        self._data = ConfigCompiler.get_values()
        
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
        route: List
        
        def set_to_null() -> None:
            for hub in self._data['hubs']:
                hub.parent = None
            self._data['end_hub'].parent = None

        def get_route(dron: Dron) -> List[Hub]:
            route: List[Hub] = []
            pos: Hub = self._data['end_hub']
            while pos != self._data['start_hub']:
                route.append((pos.name, pos._g))
                pos = pos.parent

            route = route[::-1]
            for turn in range(len(route)):
                dron.occupation.update({turn+1: route[turn]})
            return route

        def is_valid_path() -> bool:
            

        while True:
            for d in self._data['dron']:
                set_to_null()
                self.stg.perform_turn(d, self._data)
                route = get_route(d)
                if not d.move_to(route[0]):
                    continue
        self.stg.perform_turn(self._data['dron'][0], self._data)
