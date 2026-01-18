import sys

l_arg = sys.argv[1::]

i = 0
print("=== Player Score Analytics ===")
if len(l_arg) == 0:
    print("No scores provided. Usage: python3 ft_score_analytics.py <score1> <score2> ...")
else:
    for a in sys.argv[1::]:
        try:
            l_arg[i] = int(a)
            i += 1
        except Exception:
            print(f"Oops, you typed {a} instead of score !")
    print("Scores processed:", l_arg)
    print("Total players:", len(l_arg))
    print("Total score:", sum(l_arg))
    print("Average score:", sum(l_arg) / len(l_arg))
    print("High score:", max(l_arg))
    print("Low score:", min(l_arg))
    print("Score range:", max(l_arg) - min(l_arg))