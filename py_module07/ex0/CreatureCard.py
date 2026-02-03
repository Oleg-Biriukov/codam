from ex0 import Card, Enum


class EffectType(Enum):
    damage = ('damage', 'Deal 3 damage to target')
    heal = ('heal', 'Deal 3 heal to target')
    buff = ('buff', 'Permanent: +1 mana per turn')
    debuf = ('debuf', 'Permanent: -1 mana per turn')

    def __init__(self, effect: str, description: str):
        self.effect = effect
        self.description = description


class CreatureCard(Card):
    def __init__(self, name: str, cost: int, rarity: str, attack: int,
                 health: int) -> None:
        if attack < 1 or health < 1:
            raise ValueError('health or attack have to be more then 0')
        super().__init__(name, cost, rarity)
        self.type = 'Creature'
        self.attack = attack
        self.health = health

    def play(self, game_state: dict) -> dict:
        p_state = dict(
            card_played=self.name,
            mana_used=self.cost
        )
        p_state.update(game_state)
        return p_state

    def attack_target(self, target) -> dict:
        a_state = dict(
            attacker=self.name,
            target=target,
            damage_dealt=self.attack,
            combat_resolved=True
        )
        return a_state
