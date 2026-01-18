class SecurePlant():
    def __init__(self, name: str):
        self.name = name
        self.age = 0
        self.height = 0
        print("Plant created:", name)

    def set_height(self, height: int) -> None:
        if height < 0:
            print(f"Invalid operation attempted: height {height}cm [REJECTED]")
            print("Security: Negative height rejected")
            print()
            print(f"Current plant: ({self.height}cm, {self.age} days)")
            return
        print(f"Age updated: {height}cm [OK]")
        self.height = height

    def set_age(self, age: int) -> None:
        if age < 0:
            print(f"Invalid operation attempted: age {age} days [REJECTED]")
            print("Security: Negative age rejected")
            print()
            print(f"Current plant: ({self.height}cm, {self.age} days)")
            return
        print(f"Age updated: {age} days [OK]")
        self.age = age

    def get_height(self) -> int:
        return self.height

    def get_age(self) -> int:
        return self.age


if __name__ == "__main__":
    rose = SecurePlant("rose")
    rose.set_height(20)
    rose.set_age(10)
    rose.set_age(-10)
    rose.set_age(11)
    print(rose.get_age())
