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
        if name == "":
            raise PlantError("Plant name cannot be empty!")
        elif age < 0 or height < 0:
            raise PlantError("your plant age or height is negetive!")
        if name is self.plants:
            raise PlantError("plant has already exist in garden data!")
        # water and sun level
        stats = [0, 10]
        self.plants[name] = [age, height, stats]
        print(f"Added {name} successfully")

    def water_plants(self):
        print("Watering plants...")
        print("Opening watering system")
        for plant in self.plants:
            if self.water_capicity < 10:
                raise WaterError("The water capicity too low")
            self.plants[plant][2][0] += 10
            self.water_capicity -= 10
            print(f"Watering {plant} - success")

    def check_health(self):
        print("Checking plant health...")
        for plant in self.plants:
            print(f"{plant}:", end=' ')
            sun_level = self.plants[plant][2][1]
            water_level = self.plants[plant][2][0]
            if sun_level > 10 and water_level > 10:
                print("sun and water too high,max 10", end='')
                print(f"(sun: {sun_level}, water: {water_level})")
                continue
            elif sun_level > 10:
                print(f"sun too high, max 10(sun: {sun_level})")
                continue
            elif water_level > 10:
                print(f"water too high, max 10(water: {water_level})")
                continue
            print(f"OK (water: {water_level}, sun: {sun_level})")


def test_garden_management():
    print("=== Garden Management System ===")
    try:
        my_garden = GardenManager('Garden', 'Oleh')
        print()
        my_garden.add_plants("Cucumber", 10, 10)
        print()
        my_garden.add_plants("Tomato", 10, 10)
        print()
        my_garden.add_plants("", 10, 10)
    except GardenError as ge:
        print(f"Caught {type(ge).__name__}: {ge}")
    print()
    try:
        my_garden.water_plants()
    except GardenError as ge:
        print(f"Caught {type(ge).__name__}: {ge}")
    finally:
        print("Closing watering system (cleanup)")
    print()
    my_garden.check_health()
    print()
    try:
        my_garden.water_plants()
    except GardenError as ge:
        print(f"Caught {type(ge).__name__}: {ge}")
    finally:
        print("Closing watering system (cleanup)")
    print()
    my_garden.check_health()
    print("\nTesting error recovery...")
    try:
        for times in range(0, 8):
            my_garden.water_plants()
            print("Closing watering system (cleanup)\n")
    except GardenError as ge:
        print(f"Caught {type(ge).__name__}: {ge}")
    finally:
        print("Closing watering system (cleanup)")
    print("System recovered and continuing...")
    print("\nGarden management system test complete!")
