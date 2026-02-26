from data_generator import FuncMageDataGenerator as gen


def artifact_sorter(artifacts: list[dict]) -> list[dict]:
    return sorted(artifacts, reverse=True,
                  key=lambda power: power['power'])


def power_filter(mages: list[dict], min_power: int) -> list[dict]:
    return filter(lambda mage: mage['power'] >= min_power, mages)


def spell_transformer(spells: list[str]) -> list[str]:
    return list(map(lambda spell: '*' + spell + '*', spells))


def mage_stats(mages: list[dict]) -> dict:
    return {'max_power': max(mages, key=lambda i: i['power']),
            'min_power': min(mages, key=lambda i: i['power']),
            'avg_power': float(round(sum(map(lambda m: m['power'], mages)) /
                                     len(mages), 3))}


def main():
    gen_mages = gen.generate_mages()
    power_filter(gen_mages, 3)

    gen_spells = gen.generate_spells()
    gen_spells = spell_transformer(gen_spells)
    print(f'''artifact_sorter: {artifact_sorter(gen.generate_artifacts())}
power_filter: {gen_mages}
spell_transformer: {gen_spells}
mage_stats: {mage_stats(gen.generate_artifacts())}
''')


if __name__ == "__main__":
    main()
