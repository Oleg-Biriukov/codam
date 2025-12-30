def ft_seed_inventory(seed_type: str, quantity: int, unit: str):
    if (unit == "packets"):
        message = f"{quantity} packets available"
    elif (unit == "grams"):
        message = f"{quantity} grams total"
    elif (unit == "area"):
        message = f"covers {quantity} square meters"
    else:
        message = "Unknown unit type"
    print(f"{seed_type.title()} seeds: {message}")
        