from data_generator import FuncMageDataGenerator as gen


def main():
    artifact_sorter = lambda artifacts: sorted([a['power'] for a in artifacts], # noqa
                                               reverse=True)
    power_filter = lambda mages, min_power: filter(lambda mages: mages['power'] >= min_power, mages) # noqa
    spell_transformer = lambda spells: list(map(lambda spell: '*' + spell + '*', # noqa
                                                spells))
    mage_stats = lambda mages: {'max_power': max(mages), # noqa
                                'min_power': min(mages),
                                'avg_power': float(round(sum(mages) /
                                                         len(mages), 3))}
    gen_mages = gen.generate_mages()
    power_filter(gen_mages, 3)

    gen_spells = gen.generate_spells()
    gen_spells = spell_transformer(gen_spells)
    print(f'''artifact_sorter: {artifact_sorter(gen.generate_artifacts())}
power_filter: {gen_mages}
spell_transformer: {gen_spells}
mage_stats: {mage_stats(gen.generate_spell_powers())}
''')


if __name__ == "__main__":
    main()
