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
        nonlocal storage
        storage[key] = value

    def recall(key: int) -> any:
        nonlocal storage
        if storage.get(key):
            return storage[key]
        else:
            return 'Memory not found'

    return {'store': lambda key, value: store_data(key, value),
            'recall': lambda key: recall(key)}


def main():
    count = mage_counter()
    power = spell_accumulator(10)
    enchantment_factory = lambda enchantment_type: lambda enchantment_name: enchantment_type + ' ' + enchantment_name # noqa
    item = enchantment_factory('Flaming')

    print(count(), count(), count)
    print(item('sword'))
    print(power(3))
    print(power(3))
    print(memory_vault()['store'](1, 'item'))
    print(memory_vault()['recall'](1))


if __name__ == "__main__":
    main()
