def ft_count_harvest_recursive():
    left = int(input("Days until harvest: "))

    def do_recursive(day):
        if (day == left):
            print("Day", day)
            print("Harvest time!")
            return
        print("Day", day)
        do_recursive(day + 1)
    do_recursive(1)
