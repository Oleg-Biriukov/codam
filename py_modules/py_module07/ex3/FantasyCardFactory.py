import random
from ex0 import Card, CreatureCard, EffectType
from ex1 import SpellCard
from ex3.CardFactory import CardFactory


class FantasyCardFactory(CardFactory):
    '''
    Factory for all previous card, that creates each of them.
    '''
    def __init__(self):
        self.av_types = dict(
            Creatures=['dragon', 'goblin'],
            spells=['fireball'],
            artifacts=['mana_ring']
        )
        self.av_rarity = ['rare', 'common', 'ellite', 'legendary']

    def create_creature(self, name_or_power: str | int | None = None) -> Card:
        if isinstance(name_or_power, int):
            cost = name_or_power
        else:
            cost = random.choice(range(1, 10))
        if isinstance(name_or_power, str):
            name = name_or_power
        else:
            name = random.choice(self.av_types['Creatures'])
        rart = random.choice(self.av_rarity)
        attck = random.choice(range(1, 10))
        hlth = random.choice(range(1, 10))
        card = CreatureCard(name, cost, rart, attck, hlth)
        return card

    def create_spell(self, name_or_power: str | int | None = None) -> Card:
        if isinstance(name_or_power, int):
            cost = name_or_power
        else:
            cost = random.choice(range(1, 10))
        if isinstance(name_or_power, str):
            name = name_or_power
        else:
            name = random.choice(self.av_types['spells'])
        rart = random.choice(self.av_rarity)
        effect = random.choice(list(EffectType))
        card = SpellCard(name, cost, rart, effect.effect)
        return card

    def create_artifact(self, name_or_power: str | int | None = None) -> Card:
        if isinstance(name_or_power, int):
            cost = name_or_power
        else:
            cost = random.choice(range(1, 10))
        if isinstance(name_or_power, str):
            name = name_or_power
        else:
            name = random.choice(self.av_types['artifacts'])
        rart = random.choice(self.av_rarity)
        effect = random.choice(list(EffectType))
        card = SpellCard(name, cost, rart, effect.effect)
        return card

    def create_themed_deck(self, size: int) -> dict:
        deck = []
        if size >= 1:
            cards = [self.create_artifact, self.create_creature,
                     self.create_spell]
            for t in range(1, size):
                card = random.choice(cards)()
                deck.append(card)
            result = dict(
                spells=[x for x in deck if x.type == 'Spell'],
                artifact=[x for x in deck if x.type == 'Artifact'],
                creatures=[x for x in deck if x.type == 'Creature']
            )
            return result

    def get_supported_types(self) -> dict:
        return self.av_types

    def extend_card_types(self, extend: dict) -> bool:
        try:
            for type in extend.keys():
                self.av_types[type].extend(extend[type])
            return True
        except Exception:
            return False
