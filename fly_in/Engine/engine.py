from pydantic import BaseModel, PrivateAttr, ValidationError, ConfigDict
from ConfigCompiler.ConfigCompiler import ConfigCompiler
from typing import List, ClassVar, Any
from Engine.strategies import Strategy
from hubs.hub import Hub, Dron
import pygame as p
from webcolors import name_to_rgb as color  # type: ignore
import sys
from datetime import datetime as dt
from os import listdir as ls


class Camera(BaseModel):
    model_config = ConfigDict(arbitrary_types_allowed=True)
    SCREEN: ClassVar[p.Surface]
    pos: tuple[int, int] = (0, 0)
    speed: tuple[float, float] = (0.0, 0.0)
    zoom: float = 1.0
    _turns: int = PrivateAttr(0)
    font: p.font.Font
    GUIDE: ClassVar[dict] = {
        1: 'w,a,s,d - movement',
        2: 'R3 - zoom in/zoom out',
        3: 'SPACE - start',
        4: '-/= - adjusting speed of drons'
    }

    def display_info(self) -> None:
        """
made for display guid about control
"""

        offset: int = 30

        def text_creator(t: str) -> tuple:
            nonlocal offset
            text: p.Surface = self.font.render(t,
                                               True,
                                               color('lightskyblue'))
            text_r = text.get_rect()

            x: int = Engine.WIDTH // 2
            y: int = offset

            text_r.center = (x, y)
            offset += text.get_height() + 2
            return (text, text_r)

        for t in Camera.GUIDE.values():
            Camera.SCREEN.blit(*text_creator(t))

    def display_turn(self, turns: int, state: bool) -> None:
        """
displays count of turns (by default values is 0)
"""
        if state:
            self._turns = turns
        text: p.Surface = self.font.render(str(self._turns),
                                           True,
                                           color('lightskyblue'))
        text_r = text.get_rect()
        text_r.topright = (text.get_width()+20, text.get_height())
        Camera.SCREEN.blit(text, text_r)

    def error(self, txt: str) -> None:
        """
made for display error prompt in case unsovable configuration
"""
        beggining: str = 'ERROR: '
        new_font: p.font.Font = p.font.Font(Engine.FONT, 50)
        text: p.Surface = new_font.render(beggining + txt,
                                          True,
                                          color('darkred'))
        text_r = text.get_rect()
        text_r.center = (Engine.WIDTH // 2, Engine.HEIGHT // 2)
        Camera.SCREEN.blit(text, text_r)


class Engine(BaseModel):
    _data: dict = PrivateAttr()
    _screen: p.Surface = PrivateAttr()
    _clock: p.time.Clock = PrivateAttr()

    _assets: dict = PrivateAttr({
            'hub': (193, 17, 368, 308),
            'start_end': (661, 17, 408, 308),
            'dron': (1169, 115, 220, 155)
            })
    _cmr: Camera = PrivateAttr()
    _frames: dict = PrivateAttr({})

    FONT: ClassVar[str]
    WIDTH: ClassVar[int] = 1500
    HEIGHT: ClassVar[int] = 1500
    WORLD_R: ClassVar[int] = 10000
    BCKGRND: ClassVar[str] = 'assets/background/'

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
        '''
configure all data from <filename>. Also include error handling,
checking files for existing, cutting and scale all
assets for visualization for work
'''
        colored_hubs: dict = {}
        frames: list = ls(Engine.BCKGRND)

        def clring(img: p.Surface, chg: str) -> p.Surface:
            if chg == 'None':
                return img
            img.lock()
            for x in range(img.get_width()):
                for y in range(img.get_height()):
                    r, g, b, a = img.get_at((x, y))
                    if r < 18 and g < 18 and b < 18 and a > 30:
                        img.set_at((x, y), color(chg))
            img.unlock()
            return img

        ConfigCompiler.modify_path(filename)
        print(f'\n[{dt.now()}] Initialization pygame module')
        p.init()
        p.display.set_caption('fly_in')
        print(f'[{dt.now()}] Loading assets...', end=' ')

        try:
            asset = p.image.load('assets/hubs_dron.png')
            print('OK')
            print(f'[{dt.now()}] Preparing font...', end=' ')
            Engine.FONT = './assets/Minecraftia-Regular.ttf'
            with open(Engine.FONT, 'r'):
                pass
            print('OK')
            print(f'[{dt.now()}] Reading data from {filename}...',
                  end=' ')
            self._data = ConfigCompiler.get_values()
            print('OK')
            print(f'[{dt.now()}] Loading {len(frames)} background images...',
                  end=' ')
            for i, frm in enumerate(frames):
                self._frames[i] = p.image.load(Engine.BCKGRND+frm)
            print('OK')
        except Exception as e:
            print('KO')
            print(type(e).__name__, end=' ln_')
            if type(e) is ValidationError:
                print(e.errors()[0]['msg'])
            else:
                print(e)
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
                    hb.color)
            else:
                colored_hubs[hb.name] = clring(self._assets['hub'].copy(),
                                               hb.color)
        self._assets = colored_hubs

        self._screen = p.display.set_mode((Engine.WIDTH, Engine.HEIGHT),
                                          p.RESIZABLE)

        self._cmr = Camera(
            font=p.font.Font(Engine.FONT, 25)
        )

        Camera.SCREEN = self._screen

        self._cmr.pos = (Engine.WORLD_R//2, Engine.WORLD_R//2)

        self._clock = p.time.Clock()

    def start(self) -> None:
        '''
generate frame in rate for 30 frames per second and.
One frame include background picture, gray cover,
hubs positions, connection and drons if we start the game
        '''
        turns: int = 0
        font: p.font.Font
        sv_con_cap: List[list[int]] = [[n for n in hub.next]
                                       for hub in self._data['hubs']]
        is_running: bool = True
        start: bool = False
        uncompletable: bool = False
        zoom: float
        dt: float
        keys: p.key.ScancodeWrapper
        speed: float
        scale: int
        text_s: p.Surface
        spd_drn: float = 1000
        countdown = len(self._frames) - 1
        overlay: p.Surface = p.Surface((Engine.WIDTH, Engine.HEIGHT),
                                       p.SRCALPHA)

        def set_background() -> p.Surface:
            '''
            take one picture from gif for the background
            '''
            nonlocal countdown

            countdown -= 1
            if countdown < 0:
                countdown = len(self._frames) - 1
            return p.transform.scale(self._frames[countdown],
                                     (Engine.WIDTH, Engine.HEIGHT))

        def set_to_null() -> None:
            for hub in self._data['hubs']:
                hub.parent = None
                hub._g = float('inf')

        def get_route(dron: Dron) -> bool:
            '''
            create the route for dron by parent that was generated by strategy
            '''
            route: List = []
            pos: Any = self._data['end_hub']
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
            '''
            moving all drons towards hubs
            '''
            import math as m
            new_x: float
            new_y: float

            img = self._assets[0]
            r_img = img.get_rect()

            dx: float = dron.pos.pos[0] - dron.c_pos[0]
            dy: float = dron.pos.pos[1] - dron.c_pos[1]
            distance = m.hypot(dx, dy)
            if distance > 0:
                dx /= distance
                dy /= distance

            new_x = dron.c_pos[0] + dx * spd_drn * zoom * dt
            new_y = dron.c_pos[1] + dy * spd_drn * zoom * dt
            if distance < spd_drn * zoom * dt:
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
            dt = self._clock.tick(30) / 1000
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
            if keys[p.K_ESCAPE]:
                uncompletable = False
# start game
            if keys[p.K_SPACE]:
                start = True
# speed adjusting
            if keys[p.K_MINUS] and spd_drn > 500:
                spd_drn -= 100
            if keys[p.K_EQUALS] and spd_drn < 10000:
                spd_drn += 100
# scale depency
            if 1.5 <= zoom <= 2:
                scale = 400
            elif 0.2 <= zoom <= 0.5:
                scale = 50
            else:
                scale = 100


# generating signess background
            self._screen.blit(set_background(), (0, 0))

            overlay = p.transform.scale(overlay, (Engine.WIDTH, Engine.HEIGHT))
            if uncompletable:
                overlay.fill((75, 75, 75, 220))
            else:
                overlay.fill((75, 75, 75, 180))
            self._screen.blit(overlay, (0, 0))

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

                text_s = font.render(hb.name, True, color('lightgray'))
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
                if self.is_all_arrived and self.is_done:
                    for d in self._data['dron']:
                        if d.pos != self._data['end_hub']:
                            set_to_null()
                            self.stg.perform_turn(d, self._data, turns)
                            if (not get_route(d) and
                               d.pos == self._data['start_hub'] and d.id == 0):
                                uncompletable = True
                                start = False
                                break
                            d.move_to()

                        print(f'D{d.id}-{d.pos.name}', end=' ')
                    print('\n')
                    turns += 1
            else:
                start = False
                turns = 0
                for drn in self._data['dron']:
                    srx, sry = self._data['start_hub'].pos
                    drn.c_pos = (srx-1, sry-1)
                    drn.pos = self._data['start_hub']
            self._cmr.pos = (c_x, c_y)
            if uncompletable:
                self._screen.blit(overlay, (0, 0))
                self._cmr.error('you have unsolvable configuration')
            else:
                self._cmr.display_info()
                self._cmr.display_turn(turns, start)
            p.display.flip()
        p.quit()
