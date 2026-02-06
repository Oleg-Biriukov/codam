from ex1.ArtifactCard import ArtifactCard
from ex1.SpellCard import SpellCard
from ex1.Deck import Deck
from ex0 import CreatureCard


def main():
    print('''=== DataDeck Deck Builder ===\n
Building deck with different card types...
Deck stats:''', end=' ')
    deck = Deck('MyOwn')
    cards = [SpellCard('Lightning Bolt', 3, 'Common', 'damage'),
             CreatureCard('Fire Dragon', 5, 'Legendary', 7, 5),
             ArtifactCard('Mana Crystal', 2, 'Common', 5, 'buff')]
    for c in cards:
        deck.add_card(c)
    print(deck.get_deck_stats())
    print('\nDrawing and playing cards:\n')
    for c in cards:
        d_crd = deck.draw_card()
        print(f'''Drew: {d_crd.name}({d_crd.type})
Play result: {d_crd.play({})}''')
    print("\nPolymorphism in action: Same interface, \
different card behaviors!")


if __name__ == "__main__":
    main()
