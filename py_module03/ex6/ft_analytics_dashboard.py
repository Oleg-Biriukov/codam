def list_compr(data: dict) -> None:
    """Function to demonstrate the functionality of lists tools
    and way to interact with it"""
    print("\n=== List Comprehension Examples ===")
    scores = [
        data['players'][scr]['total_score']
        for scr in data['players']
    ]
    h_scorers = [
        session["player"]
        for session in data["sessions"]
        if session["duration_minutes"] > 60
    ]
    print(f'''High scorers (>2000): {scores}
Scores doubled: {[s * 2 for s in scores]}
Active players: {h_scorers}''')


def dict_compr(data: dict) -> None:
    """Function to demonstrate the functionality of dict tools
    and way to interact with it"""
    print("\n=== Dict Comprehension Examples ===")
    p_score = {
        'p_scr': {p: s['total_score'] for p, s in data['players'].items()},
        'cat_sct': dict(
            high=len([p for p in data['players']
                      if data['players'][p]['total_score'] > 8000]),
            medium=len([p for p in data['players']
                        if data['players'][p]['total_score'] > 4000 and
                        data['players'][p]['total_score'] < 8000]),
            low=len([p for p in data['players']
                    if data['players'][p]['total_score'] < 4000])
        ),
        'acvm_c': {p: len(s['achvm']) for p, s in data['players'].items()}
    }
    print(f'''Player scores: {p_score['p_scr']}
Score categories: {p_score['cat_sct']}
Achievement counts: {p_score['acvm_c']}''')


def set_compr(data: dict) -> int:
    """Function to demonstrate the functionality of set tools
    and way to interact with it"""
    print("\n=== Set Comprehension Examples ===")
    pl = [p for p in data['players']]
    pl_achv = set()
    compl = [b['mode'] for b in data['sessions']]
    a_modes = set(compl)
    for p in pl:
        pl_achv = pl_achv.union(data["players"][p]["achvm"])
    print(f'''Unique players: {set(pl)}
Unique achievements: {pl_achv}
Active modes of game: {a_modes}
''')
    return len(pl_achv)


def combined_anal(data: dict) -> None:
    """Function to print out the summary of expressed through argument data"""
    t_achv = set_compr(data)
    scores = {}
    for p in data['players']:
        scores[p] = data['players'][p]['total_score']
    for p in scores:
        if scores[p] == max(scores.values()):
            name = p
    print(f'''=== Combined Analysis ===
Total players: {len(scores)}
Total unique achievements: {t_achv}
Average score: {sum(scores.values()) / len(scores)}
Top performer: {name} ({max(scores.values())} \
points, {len(data['players'][name]['achvm'])} \
achievements)''')


if __name__ == "__main__":
    pl_data = {
        "players": {
            "alice": {
                "level": 41,
                "total_score": 2824,
                "sessions_played": 13,
                "favorite_mode": "ranked",
                "achievements_count": 5,
                "achvm":
                {'level_10', 'treasure_hunter',
                 'boss_slayer', 'speed_demon', 'perfectionist'}
            },
            "bob": {
                "level": 16,
                "total_score": 4657,
                "sessions_played": 27,
                "favorite_mode": "ranked",
                "achievements_count": 4,
                "achvm":
                {'first_kill', 'level_10', 'boss_slayer', 'collector'}
            },
            "charlie": {
                "level": 44,
                "total_score": 9935,
                "sessions_played": 21,
                "favorite_mode": "ranked",
                "achievements_count": 4,
                "achvm":
                {'first_kill', 'level_10', 'treasure_hunter', 'speed_demon'}
            },
            "diana": {
                "level": 3,
                "total_score": 1488,
                "sessions_played": 21,
                "favorite_mode": "casual",
                "achievements_count": 4,
                "achvm":
                {'first_kill', 'level_10', 'treasure_hunter', 'speed_demon'}
            },
            "eve": {
                "level": 33,
                "total_score": 1434,
                "sessions_played": 81,
                "favorite_mode": "casual",
                "achievements_count": 4,
                "achvm":
                {'first_kill', 'level_10', 'treasure_hunter', 'speed_demon'}
            },
            "frank": {
                "level": 15,
                "total_score": 8359,
                "sessions_played": 85,
                "favorite_mode": "competitive",
                "achievements_count": 4,
                "achvm":
                {'first_kill', 'level_10', 'treasure_hunter', 'speed_demon'}
            },
        },
        "sessions": [
            {
                "player": "bob",
                "duration_minutes": 94,
                "score": 1831,
                "mode": "competitive",
                "completed": False,
            },
            {
                "player": "bob",
                "duration_minutes": 32,
                "score": 1478,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "diana",
                "duration_minutes": 17,
                "score": 1570,
                "mode": "competitive",
                "completed": False,
            },
            {
                "player": "alice",
                "duration_minutes": 98,
                "score": 1981,
                "mode": "ranked",
                "completed": True,
            },
            {
                "player": "diana",
                "duration_minutes": 15,
                "score": 2361,
                "mode": "competitive",
                "completed": False,
            },
            {
                "player": "eve",
                "duration_minutes": 29,
                "score": 2985,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "frank",
                "duration_minutes": 34,
                "score": 1285,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "alice",
                "duration_minutes": 53,
                "score": 1238,
                "mode": "competitive",
                "completed": False,
            },
            {
                "player": "bob",
                "duration_minutes": 52,
                "score": 1555,
                "mode": "casual",
                "completed": False,
            },
            {
                "player": "frank",
                "duration_minutes": 92,
                "score": 2754,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "eve",
                "duration_minutes": 98,
                "score": 1102,
                "mode": "casual",
                "completed": False,
            },
            {
                "player": "diana",
                "duration_minutes": 39,
                "score": 2721,
                "mode": "ranked",
                "completed": True,
            },
            {
                "player": "frank",
                "duration_minutes": 46,
                "score": 329,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "charlie",
                "duration_minutes": 56,
                "score": 1196,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "eve",
                "duration_minutes": 117,
                "score": 1388,
                "mode": "casual",
                "completed": False,
            },
            {
                "player": "diana",
                "duration_minutes": 118,
                "score": 2733,
                "mode": "competitive",
                "completed": True,
            },
            {
                "player": "charlie",
                "duration_minutes": 22,
                "score": 1110,
                "mode": "ranked",
                "completed": False,
            },
            {
                "player": "frank",
                "duration_minutes": 79,
                "score": 1854,
                "mode": "ranked",
                "completed": False,
            },
            {
                "player": "charlie",
                "duration_minutes": 33,
                "score": 666,
                "mode": "ranked",
                "completed": False,
            },
            {
                "player": "alice",
                "duration_minutes": 101,
                "score": 292,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "frank",
                "duration_minutes": 25,
                "score": 2887,
                "mode": "competitive",
                "completed": True,
            },
            {
                "player": "diana",
                "duration_minutes": 53,
                "score": 2540,
                "mode": "competitive",
                "completed": False,
            },
            {
                "player": "eve",
                "duration_minutes": 115,
                "score": 147,
                "mode": "ranked",
                "completed": True,
            },
            {
                "player": "frank",
                "duration_minutes": 118,
                "score": 2299,
                "mode": "competitive",
                "completed": False,
            },
            {
                "player": "alice",
                "duration_minutes": 42,
                "score": 1880,
                "mode": "casual",
                "completed": False,
            },
            {
                "player": "alice",
                "duration_minutes": 97,
                "score": 1178,
                "mode": "ranked",
                "completed": True,
            },
            {
                "player": "eve",
                "duration_minutes": 18,
                "score": 2661,
                "mode": "competitive",
                "completed": True,
            },
            {
                "player": "bob",
                "duration_minutes": 52,
                "score": 761,
                "mode": "ranked",
                "completed": True,
            },
            {
                "player": "eve",
                "duration_minutes": 46,
                "score": 2101,
                "mode": "casual",
                "completed": True,
            },
            {
                "player": "charlie",
                "duration_minutes": 117,
                "score": 1359,
                "mode": "casual",
                "completed": True,
            },
        ],
        "game_modes": ["casual", "competitive", "ranked"],
        "achievements": [
            "first_kill",
            "level_10",
            "speed_demon",
            "treasure_hunter",
            "boss_slayer",
            "pixel_perfect",
            "perfectionist",
            "explorer",
        ],
    }
    list_compr(pl_data)
    dict_compr(pl_data)
    combined_anal(pl_data)
