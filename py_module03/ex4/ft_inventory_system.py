import sys


def extract_arg() -> dict:
    args = sys.argv[1::]
    arg_d = {}
    sum = 0
    for arg in args:
        arg = arg.split(':')
        arg_d[arg[0]] = int(arg[1])
        sum += int(arg[1])
    return arg_d


def invertory_anal(inv: dict) -> None:
    print("=== Inventory System Analysis ===")
    print("Total items in inventory:", sum(inv.values()))
    print("Unique item types:", len(inv.keys()))


def stats_inventory(inv: dict) -> None:
    """The item() method very usefull in for loops
to extract the information by pairs """
    one_p = float(100 / sum(inv.values()))
    print("\n=== Current Inventory ===")
    for item in inv:
        print(f"{item}: {inv[item]}", end=' ')
        print(f"units ({inv[item] * one_p}%)")
    print("\n=== Inventory Statistics ===")
    highest = 0
    smallest = max(inv.values())
    for item, count in inv.items():
        if highest < count:
            hst_itm = [item, count]
            highest = count
        if smallest >= count:
            smlt_itm = [item, count]
            smallest = count
    print(f"Most abundant: {hst_itm[0]} ({hst_itm[1]} units)")
    print(f"Least abundant: {smlt_itm[0]} ({smlt_itm[1]} units)")


def it_categories_sugg(inv: dict) -> None:
    """The most usefull thing in update() method that we can merge or even
combine it together with items() method in for loop  """
    max_c = max(inv.values())
    m_i = {}
    print("\n=== Item Categories ===")
    for item, count in inv.items():
        if count == max_c:
            m_i[item] = count
    for item in m_i:
        del inv[item]
    print(f"Moderate: {m_i}")
    print(f"Scarce: {inv}")
    inv.update(m_i)
    min_c = min(inv.values())
    restock_need = []
    if len(inv.values()) > 2:
        print("\n=== Management Suggestions ===")
        for item, count in inv.items():
            if count == min_c:
                restock_need.append(item)
        print(f"Restock needed: {restock_need}")


def d_properties(inv: dict, word: str) -> None:
    """get() method is even more usefull then update because of
we can use it in condition without raising KeyError"""
    is_exist = bool(inv.get(word))
    print("\n=== Dictionary Properties Demo ===")
    print("Dictionary keys:", list(inv.keys()))
    print("Dictionary values:", list(inv.values()))
    print(f"Sample lookup - '{word}' in inventory: {is_exist}")


if __name__ == "__main__":
    inv = extract_arg()
    invertory_anal(inv)
    stats_inventory(inv)
    it_categories_sugg(inv)
    d_properties(inv, 'sword')
