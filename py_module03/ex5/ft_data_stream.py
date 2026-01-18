it = iter(range(3))
while True:
    try:
        value = next(it)
        print(value)
    except StopIteration:
        break