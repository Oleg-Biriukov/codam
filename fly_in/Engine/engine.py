from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List, ClassVar, Self
from Engine.strategies import Strategy
from hubs.hub import Hub, Dron
import pygame as p
import sys
import datetime as dt


class Camera(BaseModel):
        pos: tuple[int, int] = (0, 0)
        zoom: float = 1.0


class Engine(BaseModel):
    _data: DataConf = PrivateAttr()
    _screen: p.Surface = PrivateAttr()
    _clock: p.Surface = PrivateAttr()
    _assets: dict = PrivateAttr({
            'hub': (193, 17, 368, 308),
            'start_end': (661, 17, 408, 308),
            'dron': (1169, 115, 220, 155)
            })
    _cmr: Camera = PrivateAttr()
    WIDTH: ClassVar[int] = 1000
    HEIGHT: ClassVar[int] = 1000

    stg: Strategy

    @property
    def is_done(self) -> bool:
        return len(list(filter(lambda x: x.pos != self._data['end_hub'],
                        self._data['dron']))) != 0

    def configure(self, filename: str) -> None:
        self._cmr = Camera()

        ConfigCompiler.modify_path(filename)
        print(f'\n[{dt.datetime.now()}] Initialization pygame module')
        p.init()
        print(f'[{dt.datetime.now()}] Loading assets...', end=' ')

        try:
            asset = p.image.load('assets/hubs_dron.png')
            print('OK')
            print(f'[{dt.datetime.now()}] Reading data from {filename}...',
                  end=' ')
            self._data = ConfigCompiler.get_values()
            print('OK')
        except Exception:
            print('KO')
            print('Shutting down ...')
            sys.exit()

        hub_p = asset.subsurface(self._assets['hub']).copy()
        start_end = asset.subsurface(self._assets['start_end']).copy()
        dron_p = asset.subsurface(self._assets['dron']).copy()

        self._assets['hub'] = p.transform.scale(hub_p, (100, 100))
        self._assets['start_end'] = p.transform.scale(start_end, (100, 100))
        self._assets['dron'] = p.transform.scale(dron_p, (30, 30))

        self._screen = p.display.set_mode((Engine.WIDTH, Engine.HEIGHT),
                                          p.RESIZABLE)
        self._clock = p.time.Clock()

    def make_turn(self) -> None:
        turns: int = 0
        sv_con_cap: List[list[int]] = [[n for n in hub.next]
                                       for hub in self._data['hubs']]
        is_running: bool = True
        zoom: float

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

        while is_running:
            zoom = self._cmr.zoom
            for event in p.event.get():
                if event.type == p.QUIT:
                    is_running = False
                if event.type == p.VIDEORESIZE:
                    Engine.WIDTH, Engine.HEIGHT = event.w, event.h
                if event.type == p.MOUSEWHEEL:
                    zoom += event.y * 0.1
                    zoom = max(0.2, min(zoom, 5))
                    self._cmr.zoom = zoom

            self._screen.fill((0, 0, 0))

            for hb in self._data['hubs']:
                img: p.Surface
                r_img: p.Rect

                x, y = hb.pos
                if hb == self._data['end_hub'] or hb == self._data['start_hub']:
                    self._assets['start_end'] = p.transform.scale(
                        self._assets['start_end'],
                        (int(100 * zoom), int(100 * zoom))
                        )

                    img = self._assets['start_end']
                else:
                    self._assets['hub'] = p.transform.scale(
                        self._assets['hub'],
                        (int(100), int(100))
                        )

                    img = self._assets['hub']

                r_img = img.get_rect()
                r_img.center = ((Engine.WIDTH // 2) + (x * int(200 * zoom)),
                                (Engine.HEIGHT // 2) + (y * int(200 * zoom)))
                self._screen.blit(img, r_img)
            if self.is_done:
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
            p.display.flip()
        p.quit()
