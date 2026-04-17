from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List
from Engine.strategies import Strategy
from hubs.hub import Hub, Dron
import pygame as p
import sys
import datetime as dt


class Engine(BaseModel):
    _data: DataConf = PrivateAttr()
    _screen: p.Surface = PrivateAttr()
    _clock: p.Surface = PrivateAttr()
    _assets: dict = PrivateAttr({
            'hub': (193, 17, 368, 308),
            'start_end': (661, 17, 408, 308),
            'dron': (1169, 115, 220, 155)
            })

    stg: Strategy

    def configure(self, filename: str) -> None:
        ConfigCompiler.modify_path(filename)
        print(f'[{dt.datetime.now()}] Reading data from {filename}...', end=' ')
        self._data = ConfigCompiler.get_values()
        print(f'\n[{dt.datetime.now()}] Initialization pygame module')
        p.init()
        print(f'[{dt.datetime.now()}] Loading assets...', end=' ')
        try:
            asset = p.image.load('assets/hubs_dron.png')
        except Exception:
            print('KO')
            print('Shutting down ...')
            sys.exit()
        print('OK')
        hub_p = asset.subsurface(self._assets['hub']).copy()
        start_end = asset.subsurface(self._assets['start_end']).copy()
        dron_p = asset.subsurface(self._assets['dron']).copy()
        self._assets['hub'] = p.transform.scale(hub_p, (100, 100))
        self._assets['start_end'] = p.transform.scale(start_end, (100, 100))
        self._assets['dron'] = p.transform.scale(dron_p, (30, 30))
        self._screen = p.display.set_mode((2000, 2000))
        self._clock = p.time.Clock()
        


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

    def make_turn(self) -> bool:
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
        while True:
        # while len(list(filter(lambda x: x.pos != self._data['end_hub'],
        #                       self._data['dron']))) != 0:
            for event in p.event.get():
                if event == p.QUIT:
                    sys.exit()

            print(f'================={turns+1}==================')
            for h in range(len(self._data['hubs'])):
                for n in range(len(self._data['hubs'][h].next)):
                    self._data['hubs'][h].next[n] = sv_con_cap[h][n]

            self._screen.fill((0, 0, 0))

            for hb in self._data['hubs']:
                x, y = hb.pos
                if hb == self._data['end_hub'] or hb == self._data['start_hub']:
                    self._screen.blit(self._assets['start_end'], (x*4000, y*4000))
                else:
                    self._screen.blit(self._assets['hub'], (x*400, y*400))

            for d in self._data['dron']:
                set_to_null()
                self.stg.perform_turn(d, self._data, turns)
                if get_route(d) and d.pos != self._data['end_hub']:
                    if not d.move_to():
                        print(f'd{d.id} -> {d.pos.name}')
                        continue
                print(f'd{d.id} -> {d.pos.name}')
            p.display.flip()
            turns += 1
        return True
