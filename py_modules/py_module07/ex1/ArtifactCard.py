from ex0 import Card, EffectType


class ArtifactCard(Card):
    '''
    ArtifactCard was based on Card class and made for giving effect
    that based on Effect type enum
    '''
    def __init__(self, name: str, cost: int, rarity: str,
                 durability: int, effect: str):
        if effect in EffectType.__members__:
            super().__init__(name, cost, rarity)
            self.type = 'Artifact'
            self.durability = durability
            self.effect = EffectType[effect]
        else:
            raise ValueError('wrong Effect')

    def play(self, game_state: dict) -> dict:
        self.durability -= 1
        p_state = dict(
            card_played=self.name,
            mana_used=self.cost,
            effect=self.effect.description
            )
        p_state.update(game_state)
        return p_state

    def activate_ability(self) -> dict:
        return dict(
            attacker=self.name,
            target='Battlefield',
            effect=self.effect.description,
            effect_resolved=True
        )
