import sys

def extract_arg() -> dict:
    args = sys.argv[1::]
    arg_d = {}
    sum = 0
    for arg in args:
        arg = arg.split(':')
        arg_d[arg[1]] = int(arg[0])
        sum += int(arg[0])
    return arg_d


def invertory_anal(inv: dict) -> None:
    print("=== Inventory System Analysis ===")
    print("Total items in inventory:", sum(inv.keys()))
    print("Unique item types:", len(inv.values()))


def current_inventory(inv: dict) -> None:
    one_p = float(100 / sum(inv.keys()))
    print("=== Current Inventory ===")
    for item in inv.values():
        print(f"{item}: {inv[item]}", end='')
        print(f"units ({inv[item] * one_p}%)")
    print()


current_inventory(extract_arg())