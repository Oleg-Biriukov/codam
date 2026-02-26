def mage_counter() -> callable:
    count = 0

    def incr() -> None:
        nonlocal count
        count += 1
        return count
    return incr


def spell_accumulator(initial_power: int) -> callable:
    total_power = initial_power

    def incr(new_power: int) -> None:
        nonlocal total_power
        total_power += new_power
        return total_power
    return incr


def memory_vault() -> dict[str, callable]:
    storage = {}

    def store_data(key: int, value: any) -> None:
        storage[key] = value

    def recall(key: int) -> any:
        return storage.get(key, 'Memory not found')

    return {'store': lambda key, value: store_data(key, value),
            'recall': lambda key: recall(key)}


def enchantment_factory(enchantment_type: str) -> callable:
    return lambda enchantment_name: enchantment_type + ' ' + enchantment_name


def main():
    count = mage_counter()
    power = spell_accumulator(10)
    item = enchantment_factory('Flaming')

    print(count(), count(), count())
    print(item('sword'))
    print(power(3))
    print(power(3))
    vault = memory_vault()
    vault['store'](1, 'item')
    print(vault['recall'](1))


if __name__ == "__main__":
    main()
