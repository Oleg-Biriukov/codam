class Plant():
    def __init__(self, name: str, height: int, age: int):
        self.name = name
        self.height = height
        self.age = age
    def print_plant(self):
        print(f"{self.name}: {self.height}cm, {self.age} days old")

if __name__ == "__main__":
    rose = Plant("Rose", 25, 30)
    sunflower = Plant("Sunflower", 80, 45)
    cactus = Plant("Cactus", 15, 120)
    print("=== Garden Plant Registry ===")
    rose.print_plant()
    sunflower.print_plant()
    cactus.print_plant()
