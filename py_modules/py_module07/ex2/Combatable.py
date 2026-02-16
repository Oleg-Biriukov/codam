from abc import ABC, abstractmethod


class Combatable(ABC):
    '''
    Abstract class for all combatable cards.
    '''
    @abstractmethod
    def attack(self, target: str) -> dict:
        pass

    @abstractmethod
    def defend(self, incoming_damage: int) -> dict:
        pass

    @abstractmethod
    def get_combat_stats(self) -> dict:
        pass
