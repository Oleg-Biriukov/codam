from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List, ClassVar, Self
from Engine.strategies import Strategy
from hubs.hub import Hub, Dron
import pygame as p
import webcolors as w
import sys
import datetime as dt


class Camera(BaseModel):
    pos: tuple[float, float] = (0, 0)
    speed: tuple[float, float] = (0.0, 0.0)
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
    WIDTH: ClassVar[int] = 1500
    HEIGHT: ClassVar[int] = 1500
    WORLD_R: ClassVar[int] = 10000

    stg: Strategy

# defining the status of drons if all arrivees then true, not False
    @property
    def is_done(self) -> bool:
        return len(list(filter(lambda x: x.c_pos != self._data['end_hub'].pos,
                        self._data['dron']))) != 0

    @property
    def is_all_arrived(self) -> bool:
        return len(list(filter(lambda x: x.c_pos == x.pos.pos,
                        self._data['dron']))) == len(self._data['dron'])

    def configure(self, filename: str) -> None:
        self._cmr = Camera()

        ConfigCompiler.modify_path(filename)
        print(f'\n[{dt.datetime.now()}] Initialization pygame module')
        p.init()
        p.display.set_caption('fly_in')
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
        self._assets['dron'] = p.transform.scale(dron_p, (50, 50))

        self._screen = p.display.set_mode((Engine.WIDTH, Engine.HEIGHT),
                                          p.RESIZABLE)
        self._cmr.pos = (Engine.WORLD_R//2, Engine.WORLD_R//2)

        self._clock = p.time.Clock()

    def make_turn(self) -> None:
        turns: int = 0
        dron_moved: int = 0
        sv_con_cap: List[list[int]] = [[n for n in hub.next]
                                       for hub in self._data['hubs']]
        is_running: bool = True
        start: bool = False
        zoom: float
        dt: float
        keys: dict
        speed: float
        scale: int

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

        def arriving_dron(dron: Dron) -> tuple:
            import math as m
            new_x: float
            new_y: float
            spd: float = 1000

            img = self._assets['dron']
            r_img = img.get_rect()

            dx = dron.pos.pos[0] - dron.c_pos[0]
            dy = dron.pos.pos[1] - dron.c_pos[1]
            distance = m.hypot(dx, dy)
            if distance > 0:
                dx /= distance
                dy /= distance

            new_x = dron.c_pos[0] + dx * spd * zoom * dt
            new_y = dron.c_pos[1] + dy * spd * zoom * dt
            if distance < spd * zoom * dt:
                dron.c_pos = dron.pos.pos
            else:
                dron.c_pos = (new_x, new_y)

            scr_x, scr_y = (int(((Engine.WORLD_R // 2) + int(dron.c_pos[0]
                                * zoom) - c_x) * zoom),
                            int(((Engine.WORLD_R // 2) + int(dron.c_pos[1]
                                * zoom) - c_y) * zoom))
            r_img.center = (scr_x, scr_y)
            return (img, r_img)

        while is_running:
            zoom = self._cmr.zoom
            c_x, c_y = self._cmr.pos
            dt = self._clock.tick(100) / 1000
            keys = p.key.get_pressed()
            speed = 1000 / zoom

# event checking
            for event in p.event.get():
                if event.type == p.QUIT:
                    is_running = False
                if event.type == p.VIDEORESIZE:
                    Engine.WIDTH, Engine.HEIGHT = event.w, event.h
                if event.type == p.MOUSEWHEEL:
                    zoom += event.y * 0.1
                    zoom = max(0.2, min(zoom, 2))
                    self._cmr.zoom = zoom
# keyboard checking
            if keys[p.K_w]:
                c_y -= dt * speed
            if keys[p.K_s]:
                c_y += dt * speed
            if keys[p.K_d]:
                c_x += dt * speed
            if keys[p.K_a]:
                c_x -= dt * speed
# start game
            if keys[p.K_SPACE]:
                start = True
# scale depency
            if 1.5 <= zoom <= 2:
                scale = 400
            elif 0.2 <= zoom <= 0.5:
                scale = 50
            else:
                scale = 100


# generating grid as background
            self._screen.fill(w.name_to_rgb('white'))
            s_x: float
            s_y: float
            for x in range(-Engine.WORLD_R, Engine.WORLD_R, 100):
                s_x = (x - c_x) * zoom + Engine.WIDTH // 2
                p.draw.line(self._screen, w.name_to_rgb('black'),
                            (s_x, 0), (s_x, Engine.HEIGHT))
            for y in range(-Engine.WORLD_R, Engine.WORLD_R, 100):
                s_y = (y - c_y) * zoom + Engine.HEIGHT // 2
                p.draw.line(self._screen, w.name_to_rgb('black'),
                            (0, s_y), (Engine.WIDTH, s_y))

# generating connection and coordinates for future hubs
            hubs_loc: list[tuple[Hub, int, int]] = []
            for hb in self._data['hubs']:
                x, y = hb.pos

                x, y = (int(((Engine.WORLD_R // 2) + int(x * zoom) - c_x)
                            * zoom),
                        int(((Engine.WORLD_R // 2) + int(y * zoom) - c_y)
                            * zoom))

# drawing connection firstly
                for n, _ in hb.next:
                    n_x, n_y = n.pos
                    n_x, n_y = (int(((Engine.WORLD_R // 2) + int(n_x * zoom)
                                     - c_x) * zoom),
                                int(((Engine.WORLD_R // 2) + int(n_y * zoom)
                                     - c_y) * zoom))
                    p.draw.line(self._screen, w.name_to_rgb('orange'),
                                (x, y), (n_x, n_y), int(10 * zoom))

# seperate layers for connection adn hubs and save them in hubs_loc
                hubs_loc.append((hb, x, y))

# putting our hub in center of the world, but take into account location of
# camera and value zoom.
            for hb, x, y in hubs_loc:

                if hb == self._data['end_hub'] or hb == self._data['start_hub']:
                    self._assets['start_end'] = p.transform.scale(
                        self._assets['start_end'],
                        (scale, scale)
                        )

                    img = self._assets['start_end']
                else:
                    self._assets['hub'] = p.transform.scale(
                        self._assets['hub'],
                        (scale, scale)
                        )

                    img = self._assets['hub']

                r_img = img.get_rect()
                r_img.center = (x, y)
                self._screen.blit(img, r_img)

            if self.is_done and start:
                for h in range(len(self._data['hubs'])):
                    for n in range(len(self._data['hubs'][h].next)):
                        self._data['hubs'][h].next[n] = sv_con_cap[h][n]

                for d in self._data['dron']:
                    self._screen.blit(*arriving_dron(d))
                if self.is_all_arrived:
                    for d in self._data['dron']:
                        set_to_null()
                        self.stg.perform_turn(d, self._data, turns)
                        if get_route(d) and d.pos != self._data['end_hub']:
                            if not d.move_to():
                                print(f'D{d.id}-{d.pos.name}', end=' ')
                                continue
                        print(f'D{d.id}-{d.pos.name}', end=' ')
                    print('\n')
            else:
                start = False
                turns = 0
                for drn in self._data['dron']:
                    srx, sry = self._data['start_hub'].pos
                    drn.c_pos = (srx-1, sry-1)
                    drn.pos = self._data['start_hub']
            self._cmr.pos = (c_x, c_y)
            p.display.flip()
        p.quit()
