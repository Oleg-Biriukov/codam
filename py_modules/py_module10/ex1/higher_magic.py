def spell() -> str:
    return 'fire'


def spell_combiner(spell1: callable, spell2: callable) -> callable:
    return (spell1(), spell2())


def power_amplifier(base_spell: callable, multiplier: int) -> callable:
    return base_spell() * multiplier


def conditional_caster(condition: callable, spell: callable) -> callable:
    if condition(6):
        return spell
    else:
        return 'Spell fizzled'


def spell_sequence(spells: list[callable]) -> callable:
    return [s() for s in spells]


def main():
    print(f'''spell_combiner(): {spell_combiner(spell, spell)}
power_amplifier(): {power_amplifier(spell, 5)}
conditional_caster(): {conditional_caster(lambda mana: mana < 5, spell)}
spell_sequence(): {spell_sequence([spell, spell, spell])}
''')


if __name__ == "__main__":
    main()
