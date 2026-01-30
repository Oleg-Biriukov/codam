from alchemy.grimoire import validate_ingredients, record_spell

print(f'''=== Circular Curse Breaking ===\n
Testing ingredient validation:
validate_ingredients("fire air"): {validate_ingredients("fire air")}
validate_ingredients("dragon scales"): {validate_ingredients("dragon scales")}

Testing spell recording with validation:
record_spell("Fireball", "fire air"): {record_spell("Fireball", "fire air")}
record_spell("Dark Magic", "shadow"): {record_spell("Dark Magic", "shadow")}

Testing late import technique:
record_spell("Lightning", "air"): {record_spell("Lightning", "air")}

Circular dependency curse avoided using late imports!
All spells processed safely!
''')