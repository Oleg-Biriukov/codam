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
        def wrappers(self, spell_name: str, power: int):
            if power >= min_power:
                return func(self, spell_name, power)
            else:
                return 'Insufficient power for this spell'
        return wrappers
    return decorator


def retry_spell(max_attempts: int) -> callable:
    def decorator(func: callable):
        attempts = 0

        @functools.wraps(func)
        def wrappers(*args, **kwargs):
            nonlocal attempts
            try:
                return func(*args, **kwargs)
            except Exception:
                attempts += 1
                if max_attempts >= attempts:
                    print(f"Spell failed, retrying... (attempt {attempts}/\
{max_attempts}")
                else:
                    return f'Spell casting failed after {max_attempts}\
 attempts'
        return wrappers
    return decorator


@retry_spell(max_attempts=3)
def check_mana(cost, mana_pool: int) -> bool:
    if cost < 0 or mana_pool < 0:
        raise ValueError('Cannot be negetive')
    if cost > mana_pool:
        return False
    return True


class MageGuild:
    def validate_mage_name(name: str) -> bool:
        count = 0
        for ltr in name:
            if ltr.isalpha() or ltr.isspace():
                count += 1
        if count < 3:
            return False
        return True

    @power_validator(min_power=10)
    @staticmethod
    def cast_spell(self, spell_name: str, power: int) -> str:
        if MageGuild.validate_mage_name(spell_name):
            return f'Successfully cast {spell_name} with {power} power'
        else:
            return 'Bad name'


def main():
    print(hello_world())
    print(check_mana(-1, 3))
    print(check_mana(-1, 3))
    print(check_mana(-1, 3))
    print(check_mana(-1, 3))
    m = MageGuild()
    print(m.cast_spell('3123123 ', 33))
    print(m.cast_spell('Spell', 3))
    print(m.cast_spell('Spell', 11))


if __name__ == '__main__':
    main()
