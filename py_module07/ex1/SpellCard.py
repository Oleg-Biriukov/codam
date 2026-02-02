from ex1 import Card, Enum


class SpellCard(Card):
    class EffectType(Enum):
        damage = 'damage'
        heal = 'heal'
        buff = 'buff'
        debuf = 'debuf'

    def __init__(self, name: str, cost: int, rarity: str, effect_type: str):
        super().__init__(name, cost, rarity)
        self.card.update({
            'type': 'Spell',
            'effect_type': effect_type
            })
        

    def play(self, game_state: dict) -> dict:
        p_state = dict(
            card_played=self.card['name'],
            mana_used=self.card['cost']
        )
        p_state.update(game_state)
        return p_state

    def resolve_effect(self, targets: list) -> dict:
        pass
