from ex2 import Card, Combatable, Magical
from random import choice


class EliteCard(Card, Combatable, Magical):
    def __init__(self, name: str, cost: int, rarity: str, attck: int,
                 health: int, t_mana: int) -> None:
        if (attck > 1 or health > 1 or t_mana > 1):
            super().__init__(name, cost, rarity)
            self.type = 'Elite'
            self.attck = attck
            self.health = health
            self.t_mana = t_mana
        else:
            raise ValueError('wrong stats was providen')

    def play(self, game_state: dict) -> dict:
        p_state = dict(
            card_played=self.name,
            mana_used=self.cost
        )
        p_state.update(game_state)
        return p_state

    def attack(self, target: str) -> dict:
        self.attack_s = dict(
            attacker=self.name,
            target=target,
            damage=self.attck,
            combat_type='melee'
        )
        return self.attack_s

    def defend(self, incoming_damage: int) -> dict:
        r_dam = choice(range(0, incoming_damage))
        if r_dam >= self.health:
            live = False
        else:
            live = True
        self.health = self.health - r_dam
        self.defent_s = dict(
            defender=self.name,
            damage_taken=r_dam,
            damage_blocked=incoming_damage - r_dam,
            still_alive=live
        )
        return self.defent_s

    def cast_spell(self, spell_name: str, targets: list[str]) -> dict:
        self.spell_s = dict(
            caster=self.name,
            spell=spell_name,
            targets=targets,
            mana_used=choice(range(1, self.t_mana))
        )
        return self.spell_s

    def channel_mana(self, amount: int) -> dict:
        t_mana = self.t_mana
        self.t_mana = self.t_mana - amount
        return dict(
            channeled=amount,
            total_mana=t_mana
        )

    def get_magic_stats(self) -> dict:
        return dict(
            mana_used=self.spell_s['mana_used'],
        )

    def get_combat_stats(self) -> dict:
        return dict(
            damage_taken=self.defent_s['damage_taken']
        )
