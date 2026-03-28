from ConfigCompiler.ConfigCompiler import ConfigCompiler
import argparse as a
from hubs.hub import Hub
from drons.dron import Dron
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
#     for h in data['hubs']:
#         print(f'''Name: {h.name}
# Position: {h.pos}
# Zone: {h.zone}
# Color: {h.color}
# Maxium drones: {h.max_drones}
# Maxium links capacity: {h.max_link_capacity}''')
#         for n in h.next:
#             print(f'''Next of {h.name}`s\n\tName: {n.name}
# \tPosition: {n.pos}
# \tZone: {n.zone}
# \tColor: {n.color}
# \tMaxium drones: {n.max_drones}
# \tMaxium links capacity: {n.max_link_capacity}
# ''')
#         for n in h.prev:
#             print(f'''Prev of {h.name}`s\n\tName: {n.name}
# \tPosition: {n.pos}
# \tZone: {n.zone}
# \tColor: {n.color}
# \tMaxium drones: {n.max_drones}
# \tMaxium links capacity: {n.max_link_capacity}
# ''')


if __name__ == '__main__':
    main()
