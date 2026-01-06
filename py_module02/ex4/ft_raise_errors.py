def check_plant_health(plant_name, water_level, sunlight_hours):
    if plant_name == "":
        raise ValueError("Plant name cannot be empty!")
    elif water_level > 10:
        raise ValueError(f"Water level {water_level} is too high (max 10")
    elif water_level < 1:
        raise ValueError(f"Water level {water_level} is too low (min 1)")
    elif sunlight_hours > 12:
        raise ValueError(f"Sunlight hours {sunlight_hours} is too high (max 10)")
    elif sunlight_hours < 2:
        raise ValueError(f" Sunlight hours {sunlight_hours} is too low (min 2)")
    print(f"Plant '{plant_name}' is healthy!")

def test_plant_checks():
    print("Testing good values...")
    check_plant_health("Tomato", 10, 10)
    print("\nTesting empty plant name...")
    try:
        check_plant_health("", 10, 10)
    except ValueError as ve:
        print(f"Error: {ve}\n")
    print("Testing bad water level...")
    try:
        check_plant_health("Tomato", -10, 10)
    except ValueError as ve:
        print(f"Error: {ve}\n")
    print("Testing bad sunlight hours...")
    try:
        check_plant_health("Tomato", 10, -10)
    except ValueError as ve:
        print(f"Error: {ve}\n")
    