def record_spell(spell_name: str, ingredients: str) -> str:
    from .validator import validate_ingredients
    rtrn = validate_ingredients(ingredients)
    if rtrn == f"{ingredients} - VALID":
        return f"Spell recorded: {spell_name} ({rtrn})"
    else:
        return f"Spell rejected: {spell_name} ({rtrn})"
