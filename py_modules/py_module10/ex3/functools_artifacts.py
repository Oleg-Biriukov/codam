import functools
import operator


def base_enchantment(power: int, element: str, target: str) -> str:
    return f'{element} enchant to {target} ({power} power)'


def spell_reducer(spells: list[int], operation: str) -> int:
    if operation == 'sum':
        return functools.reduce(operator.add, spells)
    elif operation == 'max':
        return functools.reduce(max, spells)
    elif operation == 'min':
        return functools.reduce(min, spells)
    elif operation == 'multiply':
        return functools.reduce(operator.mul, spells)


def partial_enchanter(base_enchantment: callable) -> dict[str, callable]:
    part_encht = functools.partial(base_enchantment, power=50)
    return {
        'fire_enchant': part_encht('fire', 'sword'),
        'ice_enchant': part_encht('ice', 'dagger'),
        'lightning_enchant': part_encht('lightning', 'stick')
    }


def memoized_fibonacci(n: int) -> int:
    pass


def spell_dispatcher() -> callable:
    pass


def main():
    print(spell_reducer([1, 2, 3, 4, 5], 'multiply'))


if __name__ == '__main__':
    main()
