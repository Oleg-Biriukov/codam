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
    def add_plant(self, n: str, h: int):
        self.types_t[0] += 1
        self.plants[n] = Plant(n, h)
        print(f"Added {n} to {self.owner}'s garden")
    def add_flowering_plant(self, n: str, h: int, f: str):
        self.types_t[1] += 1
        self.plants[n] = FloweringPlant(n, h, f)
        print(f"Added {n} to {self.owner}'s garden")
    def add_prize_flower(self, n: str, h: int, f: str, p: int):
        self.types_t[2] += 1
        self.plants[n] = PrizeFlower(n, h, f, p)
        print(f"Added {n} to {self.owner}'s garden")


class GardenManager:
    net = {}
    total_g = 0
    restrict = 2
    def __init__(self, admin: str, passw: str):
        self.admin = admin
        self.passw = passw
    class GardenStats:
        @staticmethod
        def plant_stats():
            for n in GardenManager.net:
                print(n)
                for garden in GardenManager.net[n]:
                    print(f"|---{garden.owner}`s garden")
                    sum = 0
                    print(f"|------Plant: {garden.types_t[0]}")
                    sum += garden.types_t[0]
                    print(f"|------Flowering: {garden.types_t[1]}")
                    sum += garden.types_t[1]
                    print(f"|------Prize: {garden.types_t[2]}")
                    sum += garden.types_t[2]
                    print(f"|------Overall: {sum} plants")
            print(f"Total gardens managed: {GardenManager.total_g}")

        
    @classmethod
    def create_garden_network(self, name: str, garden_list: list):
        lens = 0
        for garden in garden_list: lens += 1
        if lens > GardenManager.restrict:
            print(f"Amount of gardens exceed restrict ({GardenManager.restrict})")
            return
        if name is not GardenManager.net:
            self.total_g += 1
        GardenManager.net[name] = garden_list
    def set_restrict(self, amount: int):
        GardenManager.restrict = amount
        print(f"New restrict was set by {self.admin}")

if __name__ == "__main__":
    oleh_g = Garden('Oleh')
    oleh_g.add_flowering_plant("rose", 15, "blooming")
    oleh_g.add_plant("Oak", 200)
    oleh_g.add_prize_flower("redish", 6, "growing", 30)
    admin = GardenManager("Oleh", "123")
    admin.set_restrict(1)
    GardenManager.create_garden_network("City net garden", [oleh_g])
    GardenManager.GardenStats.plant_stats()
