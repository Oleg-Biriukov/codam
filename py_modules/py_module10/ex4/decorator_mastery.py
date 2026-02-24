import functools
import time


def spell_timer(func: callable) -> callable:
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        print('Casting', func.__name__)
        start = time.perf_counter()
        func(*args, **kwargs)
        end = time.perf_counter()
        print(f'Spell completed in {end-start:.6f}')
        return
    return wrapper


@spell_timer
def hello_world():
    print('Hello World')


def power_validator(min_power: int) -> callable:
    def decorator(func: callable):
        @functools.wraps(func)
        def wrappers(power, *args, **kwargs):
            if power >= min_power:
                func(power, *args, **kwargs)
            else:
                return 'Insufficient power for this spell'
        return wrappers


def retry_spell(max_attempts: int) -> callable:
    pass


class MageGuild:
    @staticmethod
    def validate_mage_name(name: str) -> bool:
        pass

    def cast_spell(self, spell_name: str, power: int) -> str:
        pass


def main():
    print(hello_world())


if __name__ == '__main__':
    main()
