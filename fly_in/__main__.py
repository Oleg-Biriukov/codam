import argparse as a
from hubs.hub import Hub, Dron
from typing import TypedDict, List
from Engine.engine import Engine
from Engine.strategies import Dijkstra


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
    eng = Engine(stg=Dijkstra())
    eng.configure(args.filename)
    eng.start()


if __name__ == '__main__':
    main()
