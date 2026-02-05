import random
from ex3 import Card, CreatureCard, SpellCard, EffectType, CardFactory


class FantasyCardFactory(CardFactory):
    def __init__(self):
        self.av_types = dict(
            Creatures=['dragon', 'goblin'],
            spells=['fireball'],
            artifacts=['mana_ring']
        )
        self.av_rarity = ['rare', 'common', 'ellite', 'legendary']

    def create_creatures(self, name_or_power: str | int | None = None) -> Card:
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

    def create_spells(self, name_or_power: str | int | None = None) -> Card:
        if isinstance(name_or_power, int):
            cost = name_or_power
        else:
            cost = random.choice(range(1, 10))
        if isinstance(name_or_power, str):
            name = name_or_power
        else:
            name = random.choice(self.av_types['spells'])
        rart = random.choice(self.av_rarity)
        effect = random.choice(EffectType.__members__)
        card = SpellCard(name, cost, rart, effect)
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
        effect = random.choice(EffectType.__members__)
        card = SpellCard(name, cost, rart, effect)
        return card

    def get_supported_types(self) -> dict:
        return self.av_types

    def extend_card_types(self, extend: dict) -> bool:
        try:
            for type in extend.keys():
                self.av_types[type].extend(extend[type])
            return True
        except Exception:
            return False
