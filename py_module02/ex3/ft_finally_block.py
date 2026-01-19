def water_plants(plant_list):
    print("Opening watering system")
    plants = ['tomato', 'lettuce', 'carrots']
    for plant in plant_list:
        if plant not in plants:
            raise ValueError(plant)
        print(f"Watering {plant}")
    print("Closing watering system (cleanup)")
    print("Watering completed successfully!")


def test_watering_system():
    print("Testing normal watering...")
    water_plants(['tomato', 'lettuce', 'carrots'])
    print("\nTesting with error...")
    try:
        water_plants(['tomato', 'None', 'lettuce', 'carrots'])
    except ValueError as v:
        print(f"Error: Cannot water {v} - invalid plant!")
    finally:
        print("Closing watering system (cleanup)")
    print("\nCleanup always happens, even with errors!")
