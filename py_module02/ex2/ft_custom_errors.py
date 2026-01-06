class GardenError(Exception):
    def __init__(self, message):
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message):
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message):
        super().__init__(message)


class Plant():
    def __init__(self, name: str, height: int, age: int):
        try:
            if height < 0:
                raise PlantError("height cannot be negetive")
        except PlantError:
            print("Height cannot be negetive")
            return
        self.name = name
        self.height = height
        self.age = age
        print(f"{self.name}: {self.height}cm, {self.age} days old")


class Conteyner():
    def __init__(self, name: str, capicity: int):
        try:
            if capicity < 0:
                raise WaterError("bad value, cannot be negetive")
        except WaterError:
            print("Capicity of water conteyner cannot be negetive")
            return
        self.name = name
        self.capicity = capicity
        
def test_custom_errors():
    tomato = Plant("Tomato", 5, 100000)
    print("Testing PlantError...")
    try:
        if tomato.age > 10:
            raise PlantError("The tomato plant is wilting!\n")
    except PlantError as p:
        print("Caught PlantError:", p)
    print("\nTesting WaterError...")
    try:
        tank = Conteyner("tank", 10000)
        if tank.capicity > 1000:
            raise WaterError("Not enough water in the tank!\n")
    except WaterError as w:
        print("Caught WaterError:", w)
    print("\nTesting catching all garden errors...")
    try:
        if tomato.age > 10:
            raise PlantError("The tomato plant is wilting!")
    except GardenError as g:
            print(f"Caught a garden error:", g)
    try:
        if tank.capicity > 1000:
            raise WaterError("Too much water in the tank!")
    except GardenError as g:
            print(f"Caught a garden error:", g)
    print("All custom error types work correctly!")
test_custom_errors()

