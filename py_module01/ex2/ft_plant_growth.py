class Plant():
    def __init__(self, name: str, height: int, ages: int):
        self.name = name
        self.height = height
        self.ages = ages
        self.days = 1

    def age(self) -> None:
        self.ages += 6
        self.days += 6

    def grow(self) -> None:
        self.age()
        self.height += 6

    def get_info(self) -> None:
        print(f"=== Day {self.days} ===")
        print(f"{self.name}: {self.height}cm, {self.ages} days old")
        if self.days > 1:
            print("Growth this week: +6cm")
        self.grow()


if __name__ == "__main__":
    plants = ["Rose", "Sunflower", "Cactus"]
    print("=== Garden Plant Registry ===")
    for i in range(0, 3):
        plants[i] = Plant(plants[i], 25 + i, 30 + i)
    for time in range(2):
        for plant in plants:
            plant.get_info()
