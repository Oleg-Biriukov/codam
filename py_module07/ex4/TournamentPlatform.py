from ex4.TournamentCard import TournamentCard
import random


class TournamentPlatform:
    def __init__(self):
        self.list_p = {}
        self.status = 'active'
        self.matches = 0

    def register_card(self, card: TournamentCard) -> str:
        if self.list_p.get(card.id):
            raise ValueError('The card has already exist')
        card.calculate_rating()
        self.list_p[card.id] = card
        return f'''{card.name} (ID: {card.id}):
- Interfaces: [Card, Combatable, Rankable]
- Rating: {card.rank}
- Record: {card.wins}-{card.losses}'''

    def create_match(self, card1_id: str, card2_id: str) -> dict:
        if self.list_p.get(card1_id) and self.list_p.get(card2_id):
            mtch = [card1_id, card2_id]
            winner = self.list_p[random.choice(mtch)]
            mtch.remove(winner.id)
            loser = self.list_p[mtch[0]]
            match_res = dict(
                winner=winner.id,
                loser=loser.id,
                winner_rating=winner.rank,
                loser_rating=loser.rank
            )
            winner.update_wins(winner.wins + 1)
            winner.rank += 30
            loser.update_losses(loser.losses + 1)
            loser.rank -= 15
            self.matches += 1
        else:
            raise ValueError('id doesn`t exist in tournament list')
        return match_res

    def get_leaderboard(self) -> list:
        pl_d = {c.rank: c
                for i, c in self.list_p.items()}
        pl_d = dict(sorted(pl_d.items(), reverse=True))
        return list(pl_d.values())

    def generate_tournament_report(self) -> dict:
        players = [c.rank for c in self.get_leaderboard()]
        report = dict(
            total_cards=len(self.list_p),
            matches_played=self.matches,
            avg_rating=sum(players) / len(players),
            platform_status=self.status
        )
        return report
