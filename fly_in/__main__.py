from ConfigCompiler.ConfigCompiler import ConfigCompiler
import argparse as a
from hubs.hub import Hub, Dron
from typing import TypedDict, List
from Engine.engine import Engine
from Engine.strategies import Astar


class DataConf(TypedDict):
    dron: List[Dron]
    start_hub: Hub | None
    end_hub: Hub | None
    hubs: List[Hub | None]


def main() -> None:
    parser: a.ArgumentParser = a.ArgumentParser(
        prog='Fly-In',
        description='Complete the efficient route\
between two points throw hubs',
        epilog='python3 main.py <config name>'
    )
    parser.add_argument('filename')
    args = parser.parse_args()
    eng = Engine(stg=Astar())
    eng.configure(args.filename)
    eng.make_turn()


if __name__ == '__main__':
    main()
# screen = pygame.display.set_mode((WIDTH, HEIGHT))
# clock = pygame.time.Clock()
# hubs_dron = pygame.image.load('assets/hubs_dron.png')
# hub = hubs_dron.subsurface(assets['hub']).copy()
# hub = pygame.transform.scale(hub, (100, 100))

# hb_react = hub.get_rect()
# running = True

# while running:
#     for event in pygame.event.get():
#         if event.type == pygame.QUIT:
#             running = False

#     hb_react = hb_react.move(speed)
#     if hb_react.left < 0 or hb_react.right > WIDTH:
#         speed[0] = -speed[0]
#     if hb_react.top < 0 or hb_react.bottom > HEIGHT:
#         speed[1] = -speed[1]

#     screen.fill(webcolors.name_to_rgb('purple'))
#     screen.blit(hub, hb_react)
#     pygame.display.flip()
#     clock.tick(60)
# pygame.quit()
