def check_temperature(temp_str):
    try:
        t = int(temp_str)
        if t >= 0 and t <= 40:
            print(f"Temperature {t}°C is perfect for plants!")
            return t
        elif t < 0:
            print(f"Error: {t}°C is too cold for plants (min 0°C)")
        else:
            print(f"Error: {t}°C is too hot for plants (max 40°C)")
    except Exception:
        print(f"Error: '{temp_str}' is not a valid number")


def test_temperature_input():
    input = [25, 'abc', 100, -50]

    for test in input:
        print("Testing temperature:", test)
        try:
            check_temperature(test)
        except Exception:
            print(f"Program crashed - {test}")
        print()
    print("All tests completed - program didn't crash!")
