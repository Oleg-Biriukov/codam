from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List
from Engine.strategies import Strategy
from hubs.hub import Hub, Dron


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
        route: List = []
        turns: int = 0
        
        def set_to_null() -> None:
            for hub in self._data['hubs']:
                hub.parent = None
                hub.is_possible = True
            self._data['end_hub'].parent = None
            self._data['start_hub'].parent = None
            self._data['end_hub'].is_possible = True

        def get_route(dron: Dron):
            route: List[Hub] = []
            pos: Hub = self._data['end_hub']
            while pos != self._data['start_hub']:
                route.append(pos)
                pos = pos.parent

            route = route[::-1]
            for turn in range(len(route)):
                dron.route.append((turn+1, route[turn]))
            return route

        def is_valid_paths(dron: Dron) -> bool:
            for turn, hub in dron.route:
                if (hub == self._data['end_hub']
                    or hub == self._data['start_hub']):
                    continue
                for d in self._data['dron']:
                    if d == dron:
                        continue
                    for t, h in d.route:
                        if t == turn and h == hub:
                            hub.is_possible = False
                            return False
            return True

        while len(self._data['dron']) != 0:
            for d in range(len(self._data['dron'])):
                while True:
                    set_to_null()
                    self.stg.perform_turn(self._data['dron'][d], self._data)
                    get_route(self._data['dron'][d])
                    for t, h in self._data['dron'][d].route: print(t, h.name)
                    print(is_valid_paths(self._data['dron'][d]))
                    if is_valid_paths(self._data['dron'][d]):
                        break

                if route == []:
                    del self._data['dron'][d]
                    continue
                print(route[0])
                if not self._data['dron'][d].move_to(route[0]):
                    continue
            turns += 1
