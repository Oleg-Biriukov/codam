from ex3.CardFactory import CardFactory
from ex3.GameStrategy import GameStrategy
import random
from typing import Generator


class GameEngine:
    '''
    Game engine that configure GameStrategy and CardFactory
    reqeiments, then simulate turn base on strategy.
    '''
    def __init__(self):
        self.report = dict(
            turns_simulated=0,
            strategy_used=None,
            total_damage=0,
            cards_created=0
        )

    def configure_engine(self, factory: CardFactory, strategy: GameStrategy) -> None:  # noqa
        self.fct = factory
        print('Factory:', type(self.fct).__name__)
        self.stg = strategy
        self.report['strategy_used'] = self.stg.get_strategy_name()
        print('Strategy:', self.stg.get_strategy_name())
        print('Available types:', self.fct.get_supported_types())
        self.targets = []
        for t in range(0, 5):
            self.targets.append(self.fct.create_creature())

    def simulate_turn(self) -> Generator[dict, None, str]:
        hand = self.fct.create_themed_deck(random.choice(range(3, 10)))
        print('Hand:', {t: [c.name for c in l] for t, l in hand.items()})
        p_targts = self.stg.prioritize_targets(self.targets)
        l_hand = [x for sub_list in hand.values() for x in sub_list]
        for t in range(0, len(hand)):
            act = next(self.stg.execute_turn(l_hand, p_targts))
            if act.get('damage_dealt'):
                self.report['total_damage'] += act['damage_dealt']
            self.report['turns_simulated'] += 1
            self.report['cards_created'] += 1
            yield act

    def get_engine_status(self) -> dict:
        return self.report
