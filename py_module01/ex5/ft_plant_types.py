class Plant():
    def __init__(self, name: str, height: int, age: int):
        self.name = name
        self.height = height
        self.age = age


class Flower(Plant):
    def __init__(self, name: str, height: int, age: int, color: str):
        super().__init__(name, height, age)
        self.color = color
        print(f"{name} (Flower), {height}cm, {age} days, {color} color")

    def bloom(self):
        print(self.name + " is blooming beautifully!")


class Tree(Plant):
    def __init__(self, name: str, height: int, age: int, trunk_diameter: int):
        super().__init__(name, height, age)
        self.trunk_diameter = trunk_diameter
        print(f"{name} (Tree), {height}cm,", end=' ')
        print(f"{age} days, {trunk_diameter}cm diameter")

    def produce_shade(self):
        diameter = self.trunk_diameter * 20 / 100
        shadow_square = int(3.1416 * (diameter / 2) ** 2)
        print(f"{self.name} provides {shadow_square} square meters of shade")


class Vegetable(Plant):
    def __init__(self, name: str, height: int, age: int,
                 harvest_season: str, nutritional_value: dict):
        super().__init__(name, height, age)
        self.harvest_season = harvest_season
        self.nutritional_value = nutritional_value
        highest = 0
        vitamine = ''
        RDI = {
            'C': 75,
            'A': 900,
            'K': 120,
            'B9': 400,
            'E': 15,
            'D': 20
        }
        print(f"{self.name} (Flower), {self.height}cm,", end=' ')
        print(f"{self.age} days, {harvest_season} harvest")
        for v in nutritional_value:
            daily_value = int(nutritional_value[v] / RDI[v] * 100)
            if daily_value > highest:
                highest = daily_value
                vitamine = v
        print(f"{name} is rich in vitamin {vitamine}")


if __name__ == "__main__":
    nut_val_tomato = {
        'C': 17,
        'A': 42,
        'K': 7.9,
        'B9': 15
    }
    rose = Flower("Rose", 25, 30, "red")
    rose.bloom()
    oak = Tree("Oak", 500, 1825, 50)
    oak.produce_shade()
    tomato = Vegetable("Tomato", 80, 90, "summer", nut_val_tomato)
