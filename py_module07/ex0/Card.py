from ex0 import Enum, ABC, abstractmethod


class EffectType(Enum):
    damage = ('damage', 'Deal 3 damage to target')
    heal = ('heal', 'Deal 3 heal to target')
    buff = ('buff', 'Permanent: +1 mana per turn')
    debuf = ('debuf', 'Permanent: -1 mana per turn')

    def __init__(self, effect: str, description: str):
        self.effect = effect
        self.description = description


class Card(ABC):
    def __init__(self, name: str, cost: int, rarity: str) -> None:
        self.name = name
        self.cost = cost
        self.rarity = rarity

    @abstractmethod
    def play(self, game_state: dict) -> dict:
        pass

    def get_card_info(self) -> dict:
        return dict(
            name=self.name,
            cost=self.cost,
            rarity=self.rarity
        )

    def is_playable(self, available_mana: int) -> bool:
        if available_mana < self.cost:
            return False
        return True
