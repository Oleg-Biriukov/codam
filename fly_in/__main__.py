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


# import pygame
# import math

# pygame.init()
# screen = pygame.display.set_mode((800, 600))
# clock = pygame.time.Clock()

# x, y = 100, 100
# target = (0, 0)
# speed = 5

# running = True
# while running:
#     for event in pygame.event.get():
#         if event.type == pygame.QUIT:
#             running = False
#         if event.type == pygame.MOUSEBUTTONDOWN:
#             target = pygame.mouse.get_pos()

#     # Direction to target
#     dx = target[0] - x
#     dy = target[1] - y

#     distance = math.hypot(dx, dy)

#     if distance > 0:
#         dx /= distance
#         dy /= distance

#         x += dx * speed
#         y += dy * speed

#     # Stop when close enough
#     if distance < speed:
#         x, y = target

#     # Draw
#     screen.fill((30, 30, 30))
#     pygame.draw.circle(screen, (255, 100, 100), (int(x), int(y)), 10)
#     pygame.draw.circle(screen, (100, 255, 100), target, 5)

#     pygame.display.flip()
#     clock.tick(60)

# pygame.quit()

# import pygame as p

# p.init()

# WIDTH = 1000
# HEIGHT = 700

# screen = p.display.set_mode((WIDTH, HEIGHT))
# p.display.set_caption("Camera Example")

# clock = p.time.Clock()

# # camera state
# camera_x = 0.0
# camera_y = 0.0
# zoom = 1.0

# running = True

# while running:

#     dt = clock.tick(60) / 1000

#     for event in p.event.get():
#         if event.type == p.QUIT:
#             running = False

#         if event.type == p.MOUSEWHEEL:
#             zoom += event.y * 0.1
#             zoom = max(0.2, min(zoom, 4))

#     keys = p.key.get_pressed()

#     speed = 600 / zoom

#     if keys[p.K_w]:
#         camera_y -= speed * dt
#     if keys[p.K_s]:
#         camera_y += speed * dt
#     if keys[p.K_a]:
#         camera_x -= speed * dt
#     if keys[p.K_d]:
#         camera_x += speed * dt

#     screen.fill((30, 30, 30))

#     # draw grid world
#     grid_size = 200
#     world_range = 4000

#     for x in range(-world_range, world_range, grid_size):
#         screen_x = (x - camera_x) * zoom + WIDTH // 2
#         p.draw.line(screen, (80, 80, 80), (screen_x, 0), (screen_x, HEIGHT))

#     for y in range(-world_range, world_range, grid_size):
#         screen_y = (y - camera_y) * zoom + HEIGHT // 2
#         p.draw.line(screen, (80, 80, 80), (0, screen_y), (WIDTH, screen_y))

#     # draw world object
#     world_x = 0
#     world_y = 0

#     screen_x = (world_x - camera_x) * zoom + WIDTH // 2
#     screen_y = (world_y - camera_y) * zoom + HEIGHT // 2

#     p.draw.circle(screen, (255, 100, 100), (int(screen_x), int(screen_y)), int(20 * zoom))

#     p.display.flip()

# p.quit()
