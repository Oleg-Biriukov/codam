class GardenError(Exception):
    def __init__(self, message):
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message):
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message):
        super().__init__(message)


class Garden():
    def __init__(self, name: str, owner: str):
        if name == owner:
            raise GardenError("The name of garden cannot be same as owner`s")
        self.name = name
        self.owner = owner
        print(f"The {name} garden was created by {owner}.")


class Plant():
    def __init__(self, name: str, height: int, age: int):
        if height < 0:
            raise PlantError("height cannot be negetive")
        self.name = name
        self.height = height
        self.age = age
        print(f"{self.name}: {self.height}cm, {self.age} days old")


class Conteyner():
    def __init__(self, name: str, capicity: int):
        if capicity < 0:
            raise WaterError("bad value, cannot be negetive")
        self.name = name
        self.capicity = capicity


def test_custom_errors():
    print("Testing PlantError...")
    try:
        Plant("Tomato", -5, 100000)
    except PlantError as p:
        print("Caught PlantError:", p)
    print("\nTesting WaterError...")
    try:
        Conteyner("tank", -1)
    except WaterError as w:
        print("Caught WaterError:", w)
    print("\nTesting catching garden errors...")
    try:
        Garden("Oleh", "Oleh")
    except GardenError as g:
        print("Caught a garden error:", g)
    print("\nTesting catching all garden errors...")
    try:
        Conteyner("Water Tank", -1)
        Garden("Oleh", "Oleh")
        Plant("Tomato", -5, 100000)
    except GardenError as g:
        print(f"Caught a garden type error({type(g).__name__}):", g)
    print("\nAll custom error types work correctly!")
