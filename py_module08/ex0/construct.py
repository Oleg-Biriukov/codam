import sys
import os
import site

no_in_venv = f'''MATRIX STATUS: You're still plugged in\n
Current Python: {sys.executable}
Virtual Environment: None detected
\nWARNING: You're in the global environment!
The machines can see everything you install.
\nTo enter the construct, run:
python -m venv matrix_env
source matrix_env/bin/activate # On Unix
matrix_env
Scripts
activate # On Windows'''

in_venv = f'''MATRIX STATUS: Welcome to the construct
\nCurrent Python: {sys.executable}
Virtual Environment: {os.path.basename(sys.prefix)}
Environment Path: {sys.prefix}
\nSUCCESS: You're in an isolated environment!
Safe to install packages without affecting
the global system.
\nPackage installation path:\n{site.getsitepackages()[0]}'''


def main():
    if sys.prefix != sys.base_prefix:
        print(in_venv)
    else:
        print(no_in_venv)


if __name__ == "__main__":
    main()
