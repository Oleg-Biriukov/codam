import importlib
import sys

modules = {'requests': 'Network access ready',
           'pandas': 'Data manipulation ready',
           'matplotlib': 'Visualization ready'}


def main():
    status = True
    if sys.prefix != sys.base_prefix:
        for p in modules:
            try:
                text = modules[p]
                modules[p] = importlib.import_module(p)
                print(f'[OK] {p} ({modules[p].__version__}) - {text}')
            except ModuleNotFoundError:
                print(f'[KO] {p} - {text}')
                status = False
        if status is False:
            sys.exit()
        data = {"x": [1, 2, 3], "y": [4, 5, 6]}
        out = modules['pandas'].DataFrame(data)
        out.plot(x='x', y='y', kind='line')
        modules['matplotlib'].pyplot.savefig("plot.png", dpi=300)
    else:
        print('<Warming> You are not in virtual enviroment !')


if __name__ == "__main__":
    main()
