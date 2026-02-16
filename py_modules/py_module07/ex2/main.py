from ex2.EliteCard import EliteCard


def main() -> None:
    print(
        '''\n=== DataDeck Ability System ===\n
EliteCard capabilities:
- Card: ['play', 'get_card_info', 'is_playable']
- Combatable: ['attack', 'defend', 'get_combat_stats']
- Magical: ['cast_spell', 'channel_mana', 'get_magic_stats']
\nPlaying Arcane Warrior (Elite Card):\n'''
    )
    ar_war = EliteCard('Arcane Warrior', 5, 'Legendary', 5, 7, 10)
    print(
        f'''Combat phase:
Attack result: {ar_war.attack("Enemy")}
\nDefense result: {ar_war.defend(5)}
Magic phase:
\nSpell cast: {ar_war.cast_spell('Fire Ball', ['Enemy1', 'Enemy2', 'Enemy3'])}
Mana channel: {ar_war.channel_mana(10)}\n
Multiple interface implementation successful!
'''
    )


if __name__ == "__main__":
    main()
