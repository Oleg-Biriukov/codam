def water_plants(plant_list):
    plants = ['tomato', 'lettuce', 'carrots']
    for plant in plant_list:
        if plant is not plants:
            raise ValueError(plant)
        print(f"Watering {plant}")
    print(f"Error: Cannot water {e} - invalid plant!")
    return
        print("Closing watering system (cleanup)")
    print("Watering completed successfully!")

def test_watering_system():
    print("Opening watering system")
    print("Testing normal watering...")
    try:
        water_plants(['tomato', 'lettuce', 'carrots'])
    except ValueError as v:
        p

