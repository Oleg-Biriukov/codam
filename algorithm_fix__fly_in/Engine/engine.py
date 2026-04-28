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
# Maxium drones: {h.max_drones}''')
#             for n, c in h.next:
#                 print(f'''Next of {h.name}`s\n\tName: {n.name}
# \tPosition: {n.pos}
# \tZone: {n.zone}
# \tColor: {n.color}
# \tMaxium drones: {n.max_drones}
# \tMaxium connection capicity to {h.name}: {c}
# ''')

    def make_turn(self) -> None:
        turns: int = 0
        sv_con_cap: List[list[int]] = [[n for n in hub.next]
                                       for hub in self._data['hubs']]

        def set_to_null() -> None:
            for hub in self._data['hubs']:
                hub.parent = None
                hub._g = float('inf')

        def get_route(dron: Dron) -> bool:
            route: List[Hub] = []
            pos: Hub = self._data['end_hub']
            while pos != d.pos:
                route.append(pos)
                pos = pos.parent
                if pos is None:
                    for i in range(len(dron.route)):
                        t, h = dron.route[i]
                        dron.route[i] = (t+1, h)
                    return False

            route = route[::-1]
            d.route = []
            for turn in range(len(route)):
                dron.route.append((turn+1, route[turn]))
            return True

        while len(list(filter(lambda x: x.pos != self._data['end_hub'],
                              self._data['dron']))) != 0:
            print(f'================={turns+1}==================')
            for h in range(len(self._data['hubs'])):
                for n in range(len(self._data['hubs'][h].next)):
                    self._data['hubs'][h].next[n] = sv_con_cap[h][n]

            for d in self._data['dron']:
                set_to_null()
                self.stg.perform_turn(d, self._data, turns)
                if get_route(d) and d.pos != self._data['end_hub']:
                    if not d.move_to():
                        print(f'd{d.id} -> {d.pos.name}')
                        continue
                print(f'd{d.id} -> {d.pos.name}')
            turns += 1
