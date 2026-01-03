class Plant():
    def __init__(self, name: str, height: int):
        self.name = name
        self.height = height


class FloweringPlant(Plant):
    def __init__(self, name: str, height: int, flowering: str):
        super().__init__(name, height)


class PrizeFlower(FloweringPlant):
    def __init__(self, name: str, height: int, flowering: str):
        super().__init__(name, height, flowering)
        print()






# if __name__ == "__main__":
#     rose = Flower("Rose", 25, 30, "red")
#     rose.bloom()
#     oak = Tree("Oak", 500, 1825, 50)
#     oak.produce_shade()
#     tomato = Vegetable("Tomato", 80, 90, "summer", nut_val_tomato)
