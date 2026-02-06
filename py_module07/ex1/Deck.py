from ex1.SpellCard import SpellCard
from ex1.ArtifactCard import ArtifactCard
from ex0 import Card, CreatureCard
from random import shuffle


class Deck:
    def __init__(self, name: str):
        self.name = name
        self.deck = []

    def add_card(self, card: Card) -> None:
        self.deck.append(card)

    def remove_card(self, card_name: str) -> bool:
        for i in range(0, len(self.deck)):
            if card_name == self.deck[i].name:
                del self.deck[i]
                return True
        return False

    def shuffle(self) -> None:
        shuffle(self.deck)

    def draw_card(self) -> Card:
        if len(self.deck) > 0:
            crd = self.deck[len(self.deck)-1]
            del self.deck[len(self.deck)-1]
            return crd
        return None

    def get_deck_stats(self) -> dict:
        deck_stat = dict(
            total_cards=len(self.deck),
            creatures=0,
            spells=0,
            artifacts=0,
            avg_cost=0
        )
        sum_cost = 0
        for card in self.deck:
            if isinstance(card, SpellCard):
                deck_stat['spells'] += 1
            elif isinstance(card, ArtifactCard):
                deck_stat['artifacts'] += 1
            elif isinstance(card, CreatureCard):
                deck_stat['creatures'] += 1
            sum_cost += card.cost
        deck_stat['avg_cost'] = sum_cost / deck_stat['total_cards']
        return deck_stat
