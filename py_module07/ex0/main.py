from ex0 import CreatureCard

if __name__ == "__main__":
    print('''=== DataDeck Card Foundation ===\n
Testing Abstract Base Class Design:\n''')
    dragon = CreatureCard('Fire Dragon', 5, 'Legendary', 7, 5)
    print("CreatureCard Info:\n", dragon.get_card_info())
    print("\nPlaying Fire Dragon with 6 mana available:")
    print("Playable:", dragon.is_playable(6))
    print("Play result:", dragon.play({'effect': 'Creature summoned to\
battlefield'}))
    print('\nFire Dragon attacks Goblin Warrior:')
    print("Atack result:", dragon.attack_target('Goblin Warrior'))
    print('\nTesting insufficient mana (3 available):')
    print("Playable:", dragon.is_playable(3))
    print("\nAbstract pattern successfully demonstrated!")