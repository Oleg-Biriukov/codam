from ex3.GameEngine import GameEngine
from ex3.FantasyCardFactory import FantasyCardFactory
from ex3.AggressiveStrategy import AggressiveStrategy


def main():
    print('''=== DataDeck Game Engine ===\n
Configuring Fantasy Card Game...''')
    game = GameEngine()
    game.configure_engine(FantasyCardFactory(), AggressiveStrategy())

    print('\nSimulating aggressive turn...')
    act = next(game.simulate_turn(), 'Game Over')
    print('Action:', act)
    print('\nGame Report:\n', game.get_engine_status())
    print('\nAbstract Factory + Strategy Pattern:\
 Maximum flexibility achieved!')


if __name__ == '__main__':
    main()
