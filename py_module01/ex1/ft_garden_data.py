class Plant():
    def __init__(self, name: str, height: int, age: int):
        self.name = name
        self.height = height
        self.age = age
        print(f"{self.name}: {self.height}cm, {self.age} days old")


if __name__ == "__main__":
    plants = ["Rose", "Sunflower", "Cactus"]
    print("=== Garden Plant Registry ===")
    for i in range(0, 3):
        plants[i] = Plant(plants[i], 25 + i, 30 + i)
