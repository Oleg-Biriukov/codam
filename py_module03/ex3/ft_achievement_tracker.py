p_achvmnts = {
    'alice': {'first_kill', 'level_10', 'treasure_hunter', 'speed_demon'},
    'bob': {'first_kill', 'level_10', 'boss_slayer', 'collector'},
    'charlie': {'level_10', 'treasure_hunter', 'boss_slayer', 'speed_demon', 'perfectionist'}
}
print("=== Achievement Tracker System ===\n")
for player in p_achvmnts:
    print(f"Player {player}: {p_achvmnts[player]}")
print("\n=== Achievement Analytics ===")
un_achvmnt = p_achvmnts['alice'].union(p_achvmnts['bob'], p_achvmnts['charlie'])
print(f"All unique achievements: {un_achvmnt}")
print(f"Total unique achievements: {len(un_achvmnt)}\n")
com_achv = p_achvmnts['alice'].intersection(p_achvmnts['bob'], p_achvmnts['charlie'])
print("Common to all players:", com_achv)
un_b_charl = p_achvmnts['bob'].union(p_achvmnts['charlie'])
r_alice = p_achvmnts['alice'].difference(un_b_charl)
un_a_charl = p_achvmnts['alice'].union(p_achvmnts['charlie'])
r_bob = p_achvmnts['bob'].difference(un_a_charl)
un_a_bob = p_achvmnts['alice'].union(p_achvmnts['bob'])
r_charlie = p_achvmnts['charlie'].difference(un_a_bob)
rare = r_alice.union(r_bob, r_charlie)
print("Rare achievements (1 player):", rare)
print()
a_v_b_comm = p_achvmnts['alice'].intersection(p_achvmnts['bob'])
alice_un = p_achvmnts['alice'].difference(p_achvmnts['bob'])
bob_un = p_achvmnts['bob'].difference(p_achvmnts['alice'])
print("Alice vs Bob common:", a_v_b_comm)
print("Alice unique:", alice_un)
print("Bob unique:", bob_un)
