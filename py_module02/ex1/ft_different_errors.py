def garden_operations():
    count_garden = {'seeds': input()}
    count = int(count_garden['seeds'])
    open("g_scores.txt")
    print(f"{count} {list(count_garden)[0]} left in warehouse")
    count = 200 / count


def test_error_types():
    temp_d = {'1': 1, '2': 2}
    try:
        try:
            print("Testing ValueError...")
            val = int("abc")
            print(val)
        except ValueError:
            print("Caught ValueError: invalid literal for int()\n")
        try:
            print("Testing ZeroDivisionError...")
            print(10/0)
        except ZeroDivisionError:
            print("Caught ZeroDivisionError: division by zero\n")
        try:
            print("Testing FileNotFoundError...")
            open("none")
        except FileNotFoundError as f:
            print(f"Caught FileNotFoundError: No such file '{f.filename}'\n")
        try:
            print("Testing KeyError...")
            print(temp_d['3'])
        except KeyError as e:
            print(f"Caught KeyError: {e}\n")
        print("Testing multiple errors together...")
        print(5/0)
    except (ValueError, ZeroDivisionError, FileNotFoundError, KeyError):
        print("Caught an error, but program continues!\n")
    print("All error types tested successfully!")
