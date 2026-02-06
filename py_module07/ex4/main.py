from ex4.TournamentPlatform import TournamentPlatform
from ex4.TournamentCard import TournamentCard


def main():
    pl1 = TournamentCard('dragon_001', 'Fire Dragon', 5, 'Rare', 5)
    pl2 = TournamentCard('wizard_001', 'Ice Wizard', 5, 'Rare', 5)
    print('''\n=== DataDeck Tournament Platform ===
\nRegistering Tournament Cards...\n''')
    touranment = TournamentPlatform()
    print(f'{touranment.register_card(pl1)}\n')
    print(f'{touranment.register_card(pl2)}\n')
    print('Creating tournament match...\nMatch result:',
          touranment.create_match(pl1.id, pl2.id))
    l_pl = touranment.get_leaderboard()
    for i in range(0, len(l_pl)):
        print(f'{i+1}. {l_pl[i].name} - Rating: \
{l_pl[i].rank} ({l_pl[i].wins}-{l_pl[i].losses})')
    print('\nPlatform Report:')
    print(touranment.generate_tournament_report())


if __name__ == "__main__":
    main()
