class Plant():
    def __init__(self, name: str, height: int):
        self.name = name
        self.height = height


class FloweringPlant(Plant):
    def __init__(self, name: str, height: int, flowering: str):
        super().__init__(name, height)
        self.flowering = flowering


class PrizeFlower(FloweringPlant):
    def __init__(self, name: str, height: int, flowering: str, prize: int):
        super().__init__(name, height, flowering)
        self.prize = prize


class Garden:
    def __init__(self, own_name: str):
        self.owner = own_name
        self.plants = {}
        self.types_t = [0, 0, 0]

    def add_plant(self, n: str, h: int) -> None:
        self.types_t[0] += 1
        self.plants[n] = Plant(n, h)
        print(f"Added {n} to {self.owner}'s garden")

    def add_flowering_plant(self, n: str, h: int, f: str) -> None:
        self.types_t[1] += 1
        self.plants[n] = FloweringPlant(n, h, f)
        print(f"Added {n} to {self.owner}'s garden")

    def add_prize_flower(self, n: str, h: int, f: str, p: int) -> None:
        self.types_t[2] += 1
        self.plants[n] = PrizeFlower(n, h, f, p)
        print(f"Added {n} to {self.owner}'s garden")


class GardenManager:
    net = {}
    total_g = 0
    rct = 2

    def __init__(self, admin: str, passw: str):
        self.admin = admin
        self.passw = passw

    class GardenStats:
        @staticmethod
        def plant_stats() -> None:
            for n in GardenManager.net:
                print(n)
                for garden in GardenManager.net[n]:
                    sum_prizes = 0
                    for p in garden.plants:
                        if type(garden.plants[p]).__name__ == "PrizeFlower":
                            sum_prizes += garden.plants[p].prize
                        else:
                            sum_prizes += 1
                    print(f"|---{garden.owner}`s garden")
                    sum = 0
                    print(f"|------Plant: {garden.types_t[0]}")
                    sum += garden.types_t[0]
                    print(f"|------Flowering: {garden.types_t[1]}")
                    sum += garden.types_t[1]
                    print(f"|------Prize: {garden.types_t[2]}")
                    sum += garden.types_t[2]
                    print(f"|------Total Prize: {sum_prizes}")
                    print(f"|------Overall: {sum} plants")
            print(f"Total gardens managed: {GardenManager.total_g}")

    @classmethod
    def create_garden_network(self, name: str, garden_list: list) -> None:
        lens = 0
        for garden in garden_list:
            lens += 1
        if lens > GardenManager.rct:
            print(f"Amount of gardens exceed restrict ({GardenManager.rct})")
            return
        GardenManager.total_g += len(garden_list)
        GardenManager.net[name] = garden_list

    @staticmethod
    def is_good_name(name: str) -> int:
        if len(name) > 15:
            return 0
        return 1

    def set_restrict(self, amount: int) -> None:
        GardenManager.rct = amount
        print(f"New restrict was set by {self.admin}")

    def change_name(self, new_name: str) -> None:
        if GardenManager.is_good_name(new_name):
            self.admin = new_name
            print("New name was changed")
            return
        else:
            print("You have bad name")


if __name__ == "__main__":
    oleh_g = Garden('Oleh')
    oleh_g.add_flowering_plant("rose", 15, "blooming")
    oleh_g.add_plant("Oak", 200)
    oleh_g.add_prize_flower("redish", 6, "growing", 30)
    max_g = Garden('Max')
    max_g.add_flowering_plant("rose", 15, "blooming")
    max_g.add_plant("Oak", 200)
    max_g.add_prize_flower("redish", 6, "growing", 30)
    if GardenManager.is_good_name("Oleh"):
        print("Yes is good")
    else:
        print("No is not good")
    admin = GardenManager("Oleh1", "123")
    admin.change_name("Oleh")
    admin.set_restrict(3)
    GardenManager.create_garden_network("City net garden", [oleh_g, max_g])
    GardenManager.GardenStats.plant_stats()
