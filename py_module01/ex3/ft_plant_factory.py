class Plant():
    def __init__(self, name: str, starting_height: int, starting_age: int):
        self.name = name
        self.exect_height = starting_height
        self.exect_age = starting_age
        self.starting_height = starting_height
        self.starting_age = starting_age
        print(f"Created: {self.name} ({self.exect_height}cm, {self.exect_age} days)")

if __name__ == "__main__":
    factory_in = {
        "Rose": [25, 30],
        "Oak": [200, 365],
        "Cactus": [5, 90],
        "Sunflower": [80, 45],
        "Fern": [12, 120],
        "Dieffenbachia": [30, 120]
         }
    factory_out = {}
    print("=== Plant Factory Output ===")
    for pln in factory_in:
        factory_out[pln] = Plant(pln, factory_in[pln][0], factory_in[pln][1])
    print()
    print("Total plants created:", 6)