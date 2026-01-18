import sys

print("=== Command Quest ===")
arg = sys.argv
argc = len(arg)
if argc == 1:
    print("No arguments provided!")
    print("Program name:", arg[0])
else:
    print("Program name:", arg[0])
    print("Arguments received:", argc - 1)
    arg = arg[1::]
    num_arg = 1
    for a in arg:
        print(f"Argument {num_arg}: {a}")
        num_arg += 1
print("Total arguments:", argc)


