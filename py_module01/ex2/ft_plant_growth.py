class Plant():
    def __init__(self, name: str, height: int, ages: int):
        self.name = name
        self.height = height
        self.ages = ages
        self.days = 1
    def age(self):
        self.ages += 6
        self.days += 6
    def grow(self):
        self.age()
        self.height += 6
    def get_status(self):
        print(f"=== Day {self.days} ===")
        print(f"{self.name}: {self.height}cm, {self.ages} days old")
        if self.days > 1:
            print("Growth this week: +6cm")
        self.grow()


if __name__ == "__main__":
    rose = Plant("Rose", 25, 30)
    rose.get_status()
    rose.get_status()
    rose.get_status()
    rose.get_status()
    rose.get_status()