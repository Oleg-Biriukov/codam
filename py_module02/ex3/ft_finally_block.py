def water_plants(plant_list):
    plants = ['tomato', 'lettuce', 'carrots']
    print("Opening watering system")
    try:
        for plant in plant_list:
            if plant is not plants:
                raise ValueError(plant)
            print(f"Watering {plant}")
    except ValueError as e:
        print(f"Error: Cannot water {e} - invalid plant!")
        return
    finally:
        print("Closing watering system (cleanup)")
    print("Watering completed successfully!")
water_plants(['None', 'tomato', 'lettuce', 'carrots'])

