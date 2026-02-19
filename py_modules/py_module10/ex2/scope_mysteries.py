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
    pass


def main():
    count = mage_counter()
    power = spell_accumulator(10)
    enchantment_factory = lambda enchantment_type: lambda enchantment_name: enchantment_type + ' ' + enchantment_name # noqa
    item = enchantment_factory('Flaming')
    print(item('sword'))
    print(power(3))
    print(power(3))


if __name__ == "__main__":
    main()
