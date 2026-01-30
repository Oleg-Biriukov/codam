def validate_ingredients(ingredients: str) -> str:
    valid = ["fire", "water", "earth", "air"]
    for el in valid:
        if el in ingredients:
            return f"{ingredients} - VALID"
    return f"{ingredients} - INVALID"
