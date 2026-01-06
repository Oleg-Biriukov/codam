class GardenError(Exception):
    def __init__(self, message):
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message):
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message):
        super().__init__(message)


class GardenManager:
    def __init__(self, name: str, owner: str):
        print("Creating new garden...")
        try:
            if owner == name:
                raise GardenError("The garden`s name cannot be same as owner")
        except GardenError as ge:
            print("Error creating garden:", ge)
            return
        self.owner = owner
        self.name = name
        self.water_capicity = 100
        self.plants = {}
        print("The garden was created ")
    def add_plants(self, name: str, age: int, height: int):
        print("Adding plants to garden")
        try:
            if name == "":
                raise PlantError("Plant name cannot be empty!")
        except PlantError as pe:
            print("Error adding plant:", pe)
            return
        # water and sun level
        stats = [0, 10]
        self.plants[name] = [age, height, stats]
        print(f"Added {name} successfully")
    def water_plants(self):
        print("Watering plants...")
        print("Opening watering system")
        try:
            for plant in self.plants:
                if self.water_capicity < 10: 
                    raise WaterError("The water capicity too low")
                self.plants[plant][2][0] += 10
                self.water_capicity -= 10
                print(f"Watering {plant} - success")
        except WaterError as we:
            print("Error water plants:", we)
        finally:
            print("Closing watering system (cleanup)")
    def check_health(self):
        print("Checking plant health...")
        for plant in self.plants:
            print(f"{plant}:", end = ' ')
            sun_level = self.plants[plant][2][1]
            water_level = self.plants[plant][2][0]
            if sun_level > 10 and water_level > 10:
                print(f"sun and water too high, max 10(sun: {sun_level}, water: {water_level})")
                continue
            elif sun_level > 10:
                print(f"sun too high, max 10(sun: {sun_level})")
                continue
            elif water_level > 10:
                print(f"water too high, max 10(water: {sun_level})")
                continue
            print(f"OK (water: {water_level}, sun: {sun_level})")

my_garden = GardenManager('Garden', 'Oleh')
print()
my_garden.add_plants("Cucumber", 10, 10)
print()
my_garden.add_plants("Tomato", 10, 10)
print()
my_garden.add_plants("", 10, 10)
print()
my_garden.water_plants()
print()
my_garden.check_health()
print()
my_garden.water_plants()
print()
my_garden.check_health()

    