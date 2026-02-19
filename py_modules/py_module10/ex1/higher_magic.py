def main():
    spell = lambda: 'fire' # noqa
    spells = [spell, spell, spell]
    cond = lambda spell: len(spell) < 10 # noqa
    spell_combiner = lambda spell1, spell2: (spell1(), # noqa
                                             spell2())
    power_amplifier = lambda base_spell, multiplier: base_spell() * multiplier # noqa
    conditional_caster = lambda condition, spell: spell() if condition(spell()) else 'Spell fizzled' # noqa
    spell_sequence = lambda spells: [spell() for spell in spells] # noqa
    print(f'''spell_combiner(): {spell_combiner(spell, spell)}
power_amplifier(): {power_amplifier(spell, 5)}
conditional_caster(): {conditional_caster(cond, spell)}
spell_sequence(): {spell_sequence(spells)}
''')


if __name__ == "__main__":
    main()
