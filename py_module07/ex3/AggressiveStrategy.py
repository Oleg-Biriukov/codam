from ex3.GameStrategy import GameStrategy
from ex1 import SpellCard
from typing import Generator


class AggressiveStrategy(GameStrategy):
    def execute_turn(self, hand: list, battlefield: list) -> Generator[dict, None, str]:  # noqa
        for card in hand:
            if card.type == 'Creature':
                if card.cost < 5:
                    hand.remove(card)
                    card.play({'strategy': self.get_strategy_name()})
                    l_targets = [card.name for card in battlefield]
                    yield card.attack_target(l_targets)
        for card in hand:
            if isinstance(card, SpellCard):
                hand.remove(card)
                card.play({'strategy': self.get_strategy_name()})
                l_targets = [card.name for card in battlefield]
                yield card.resolve_effect(l_targets)
        for card in hand:
            hand.remove(card)
            card.play({'strategy': self.get_strategy_name()})
            yield card.activate_ability()

    def get_strategy_name(self) -> str:
        return 'AggressiveStrategy'

    def prioritize_targets(self, available_targets: list) -> list:
        targets = []
        for card in available_targets:
            if card.type == 'Creature':
                targets.append(card)
        return targets
