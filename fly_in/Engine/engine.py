from pydantic import BaseModel, PrivateAttr
from ConfigCompiler.ConfigCompiler import DataConf, ConfigCompiler
from typing import List, ClassVar, Any
from Engine.strategies import Strategy
from hubs.hub import Hub, Dron
import pygame as p
from webcolors import name_to_rgb as color
import sys
import datetime as dt
import functools


class Camera(BaseModel):
    SCREEN: ClassVar[p.Surface]
    pos: tuple[int, int] = (0, 0)
    speed: tuple[float, float] = (0.0, 0.0)
    zoom: float = 1.0
    font: Any
    GUIDE: ClassVar[dict] = {
        1: 'w,a,s,d - movement',
        2: 'R3 - zoom in/zoom out',
        3: 'SPACE - start'
    }

    def display_info(self) -> None:
        offset: int = 0
        max_w: int = 0

        def text_creator(t: str) -> tuple:
            nonlocal offset
            text: p.Surface = self.font.render(t,
                                               True,
                                               color('lightskyblue'))
            text_r = text.get_rect()

            x: int = Engine.WIDTH - text.get_width()
            y: int = Engine.HEIGHT // 2

            text_r.center = (x, y+offset)
            offset += text.get_height() + 10
            return (text, text_r)

        for t in Camera.GUIDE.values():
            Camera.SCREEN.blit(*text_creator(t))


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
    _bckgr: p.Surface = PrivateAttr()
    _scale_bck: int = PrivateAttr(300)
    FONT: ClassVar[str]
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
        colored_hubs: dict = {}

        def clring(img: p.Surface, chg: str) -> p.Surface:
            img.lock()
            for x in range(img.get_width()):
                for y in range(img.get_height()):
                    r, g, b, a = img.get_at((x, y))
                    if r < 18 and g < 18 and b < 18 and a > 30:
                        img.set_at((x, y), color(chg))
            img.unlock()
            return img

        ConfigCompiler.modify_path(filename)
        print(f'\n[{dt.datetime.now()}] Initialization pygame module')
        p.init()
        p.display.set_caption('fly_in')
        print(f'[{dt.datetime.now()}] Loading assets...', end=' ')

        try:
            asset = p.image.load('assets/hubs_dron.png')
            self._bckgr = p.image.load('assets/clouds.png')
            print('OK')
            print(f'[{dt.datetime.now()}] Preparing font...', end=' ')
            Engine.FONT = './assets/Minecraftia-Regular.ttf'
            with open(Engine.FONT, 'r'):
                pass
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

        colored_hubs[0] = self._assets['dron']

        for hb in self._data['hubs']:
            if (hb == self._data['start_hub'] or
                    hb == self._data['end_hub']):
                colored_hubs[hb.name] = clring(
                    self._assets['start_end'].copy(),
                    hb.color.value)
            else:
                colored_hubs[hb.name] = clring(self._assets['hub'].copy(),
                                               hb.color.value)
        self._assets = colored_hubs
        print(colored_hubs)

        self._screen = p.display.set_mode((Engine.WIDTH, Engine.HEIGHT),
                                          p.RESIZABLE)

        self._cmr = Camera(
            font=p.font.Font(Engine.FONT, 30)
        )

        Camera.SCREEN = self._screen

        self._cmr.pos = (Engine.WORLD_R//2, Engine.WORLD_R//2)

        self._clock = p.time.Clock()

    def make_turn(self) -> None:
        turns: int = 1
        font: p.font.Font
        sv_con_cap: List[list[int]] = [[n for n in hub.next]
                                       for hub in self._data['hubs']]
        is_running: bool = True
        start: bool = False
        zoom: float
        dt: float
        keys: dict
        speed: float
        scale: int
        text_s: p.Surface

        def sighness_bck() -> None:
            s_x: float
            s_y: float
            scl: int = int(self._scale_bck * zoom)
            bckgr = p.transform.scale(self._bckgr,
                                      (scl, scl))

            for y in range(-Engine.WORLD_R, Engine.WORLD_R, scl):
                s_y = (y - c_y) * zoom
                for x in range(-Engine.WORLD_R, Engine.WORLD_R, scl):
                    s_x = (x - c_x) * zoom
                    self._screen.blit(bckgr, (s_x, s_y))

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

            img = self._assets[0]
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
            font = p.font.Font(Engine.FONT, int(30.0 * zoom))
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
                c_y -= int(dt * speed)
            if keys[p.K_s]:
                c_y += int(dt * speed)
            if keys[p.K_d]:
                c_x += int(dt * speed)
            if keys[p.K_a]:
                c_x -= int(dt * speed)
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


# generating signess background
            # sighness_bck()
            self._screen.fill((79, 79, 79, 128))
# grid on a layer up from background
            for x in range(-Engine.WORLD_R, Engine.WORLD_R, 100):
                s_x = (x - c_x) * zoom
                p.draw.line(self._screen, (0, 0, 0, 125),
                            (s_x, 0), (s_x, Engine.HEIGHT))
            for y in range(-Engine.WORLD_R, Engine.WORLD_R, 100):
                s_y = (y - c_y) * zoom
                p.draw.line(self._screen, (0, 0, 0, 125),
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
                    p.draw.line(self._screen, color('orange'),
                                (x, y), (n_x, n_y), int(10 * zoom))

# seperate layers for connection adn hubs and save them in hubs_loc
                hubs_loc.append((hb, x, y))

# putting our hub in center of the world, but take into account location of
# camera and value zoom.
            for hb, x, y in hubs_loc:
                self._assets[hb.name] = p.transform.scale(
                    self._assets[hb.name],
                    (scale, scale)
                    )

                img = self._assets[hb.name]

                r_img = img.get_rect()
                r_img.center = (x, y)
                self._screen.blit(img, r_img)

                text_s = font.render(hb.name, True, color('black'))
                self._screen.blit(text_s,
                                  text_s.get_rect(center=(x, y+scale-30)))

                if (hub_d := len([d for d in self._data['dron']
                                 if d.c_pos == hb.pos])) > 1:
                    text_s = font.render(str(hub_d), True, color('maroon'))
                    self._screen.blit(text_s,
                                      text_s.get_rect(
                                          center=(x, y-int(70*zoom))))

            if self.is_done and start:
                for h in range(len(self._data['hubs'])):
                    for n in range(len(self._data['hubs'][h].next)):
                        self._data['hubs'][h].next[n] = sv_con_cap[h][n]

                for d in self._data['dron']:
                    self._screen.blit(*arriving_dron(d))
                if self.is_all_arrived:
                    print(f'{turns}.', end=' ')
                    for d in self._data['dron']:
                        if d.pos != self._data['end_hub']:
                            set_to_null()
                            self.stg.perform_turn(d, self._data, turns)
                            get_route(d)
                            d.move_to()
                        print(f'D{d.id}-{d.pos.name}', end=' ')
                    print('\n')
                    turns += 1
            else:
                start = False
                turns = 1
                for drn in self._data['dron']:
                    srx, sry = self._data['start_hub'].pos
                    drn.c_pos = (srx-1, sry-1)
                    drn.pos = self._data['start_hub']
            self._cmr.display_info()
            self._cmr.pos = (c_x, c_y)
            p.display.flip()
        p.quit()
