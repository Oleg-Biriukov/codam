import functools
import operator


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
    def base_enchantment(power: int, element: str, target: str) -> str:
        return f'{element} enchant to {target} ({power} power)'

    part_encht = functools.partial(base_enchantment, power=50)
    return {
        'fire_enchant': part_encht('fire', 'sword'),
        'ice_enchant': part_encht('ice', 'dagger'),
        'lightning_enchant': part_encht('lightning', 'stick')
    }


@functools.lru_cache
def memoized_fibonacci(n: int) -> int:
    if n < 2:
        return n
    return memoized_fibonacci(n-1) + memoized_fibonacci(n-2)


def spell_dispatcher() -> callable:
    @functools.singledispatch
    def spell_system(value):
        return f'Error: wrong arg({value})'

    @spell_system.register
    def _(value: int):
        return f'The damage was dealt in amount {value}'

    @spell_system.register
    def _(value: str):
        return f'The {value} was cast to a target'

    @spell_system.register
    def _(value: list):
        return f'The {value} spells was cast to a target'

    return spell_system


def main():
    print(spell_reducer([1, 2, 3, 4, 5], 'multiply'))


if __name__ == '__main__':
    main()
