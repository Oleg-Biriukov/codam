from ex0 import Card
from ex2 import Combatable
from ex4.Rankable import Rankable
import random


class TournamentCard(Card, Combatable, Rankable):
    def __init__(self, id: str, name: str, cost: int, rarity: str,
                 attck: int) -> None:
        if attck >= 1:
            self.id = id
            super().__init__(name, cost, rarity)
            self.type = 'TournamentCard'
            self.attck = attck
            self.rank = 0
            self.wins = 0
            self.losses = 0
            self.r_info = {}
        else:
            raise ValueError('wrong arguments was providen')

    def play(self, game_state: dict) -> dict:
        p_state = dict(
            card_played=self.name,
            mana_used=self.cost
        )
        p_state.update(game_state)
        return p_state

    def attack(self, target) -> dict:
        self.attack_s = dict(
            attacker=self.name,
            target=target,
            damage=self.attck,
            combat_type='melee'
        )
        return self.attack_s

    def defend(self, incoming_damage: int) -> dict:
        r_dam = random.choice(range(0, incoming_damage))
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

    def get_combat_stats(self) -> dict:
        return dict(
            damage_dealed=self.attack_s['damage'],
            damage_taken=self.defent_s['damage_taken']
        )

    def calculate_rating(self) -> int:
        self.rank = random.choice(range(500, 600))
        return self.rank

    def update_wins(self, wins: int) -> None:
        self.wins = wins

    def update_losses(self, losses: int) -> None:
        self.losses = losses

    def get_rank_info(self) -> dict:
        self.r_info = dict(
            id=self.id,
            rank=self.rank
        )

    def get_tournament_stats(self) -> dict:
        t_stats = dict(
            losses=self.losses,
            wins=self.wins
        )
        t_stats.update(self.r_info)
        return t_stats
