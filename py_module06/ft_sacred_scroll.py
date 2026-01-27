import alchemy

print('''=== Sacred Scroll Mastery ===\n
Testing direct module access:''')
print("alchemy.elements.create_fire():", alchemy.elements.create_fire())
print("alchemy.elements.create_water():", alchemy.elements.create_water())
print("alchemy.elements.create_air():", alchemy.elements.create_air())
print("alchemy.elements.create_earth():", alchemy.elements.create_earth())

print("\nTesting package-level access (controlled by __init__.py):")
print("alchemy.create_fire():", alchemy.create_fire())
print("alchemy.create_water():", alchemy.create_water())
try:
    print("alchemy.create_air():", end=' ')
    print(alchemy.create_air())
except AttributeError:
    print("AttripwdbuteError - not exposed")
try:
    print("alchemy.create_air():", end=' ')
    print(alchemy.create_air())
except AttributeError:
    print("AttripwdbuteError - not exposed")

print(f'''\nPackage metadata:
Version: {alchemy.__version__}
Author: {alchemy.__author__}''')
