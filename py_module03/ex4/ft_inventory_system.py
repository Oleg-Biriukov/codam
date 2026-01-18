types_data = ["weapon","consumable","armor"]
alice_inv = {
    "sword": dict(quent = 1, t = ("weapon", "rare"), val = 500),
    "potion": dict(quent = 5, t = ("consumable", "common"), val = 50),
    "shield": dict(quent = 1, t = ("armor", "uncommon"), val = 200)
}
bob_inv = {
    "dagger": dict(quent = 1, t = ("weapon", "rare"), val = 300),
    "bread": dict(quent = 5, t = ("consumable", "common"), val = 25),
    "gloves": dict(quent = 1, t = ("armor", "uncommon"), val = 150)
}
anal_table = {0:}
print("=== Player Inventory System ===")
print("\n=== Alice's Inventory ===")
sum_val = 0
count = 0

for item in alice_inv.keys():
    quent = alice_inv[item]['quent']
    val = alice_inv[item]['val']
    print(f"{item}", end=" ")
    print(f"({alice_inv[item]['t'][0]}, {alice_inv[item]['t'][1]}):", end=' ')
    print(f"{quent}x @ {val}", end=' ')
    print(f"gold each = {quent * val} gold")
    sum_val += quent * val
    count += 1 * quent
print(f"\nInventory value: {sum_val} gold")
print(f"Item count: {count} items")
print(f"Categories: ", end='')
for t_d in types_data:
    counter = 0
    for stats in alice_inv.values():
        if stats['t'][0] == t_d:
            counter += 1 * stats['quent']
    if counter != 0:
        print(f"{t_d} ({counter})", end = ' ')

