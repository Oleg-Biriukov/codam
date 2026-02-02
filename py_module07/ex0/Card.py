from abc import ABC, abstractmethod


class Card(ABC):
    def __init__(self, name: str, cost: int, rarity: str) -> None:
        self.card = dict(
            name=name,
            cost=cost,
            rarity=rarity
        )

    @abstractmethod
    def play(self, game_state: dict) -> dict:
        pass

    def get_card_info(self) -> dict:
        return self.card

    def is_playable(self, available_mana: int) -> bool:
        if available_mana < self.card['cost']:
            return False
        return True
