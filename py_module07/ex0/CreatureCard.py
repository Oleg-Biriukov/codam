from ex0.Card import Card


class CreatureCard(Card):
    def __init__(self, name: str, cost: int, rarity: str, attack: int,
                 health: int) -> None:
        if attack < 1 or health < 1:
            raise ValueError('health or attack have to be more then 0')
        super().__init__(name, cost, rarity)
        self.card.update({
            'type': 'Creature',
            'attack': attack,
            'health': health
            })

    def play(self, game_state: dict) -> dict:
        p_state = dict(
            card_played=self.card['name'],
            mana_used=self.card['cost']
        )
        p_state.update(game_state)
        return p_state

    def attack_target(self, target) -> dict:
        a_state = dict(
            attacker=self.card['name'],
            target=target,
            damage_dealt=self.card['attack'],
            combat_resolved=True
        )
        return a_state
