from ex0 import Card, EffectType


class SpellCard(Card):
    def __init__(self, name: str, cost: int, rarity: str, effect_type: str):
        if effect_type in EffectType.__members__:
            super().__init__(name, cost, rarity)
            self.type = 'Spell'
            self.effect = EffectType[effect_type]
        else:
            raise ValueError('wrong Effect')

    def play(self, game_state: dict) -> dict:
        p_state = dict(
            card_played=self.name,
            mana_used=self.cost,
            effect=self.effect.description
        )
        p_state.update(game_state)
        return p_state

    def resolve_effect(self, targets: list) -> dict:
        return dict(
            attacker=self.name,
            target=targets,
            effect=self.effect.description,
            effect_resolved=True
        )
