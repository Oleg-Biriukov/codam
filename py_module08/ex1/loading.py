import importlib
import sys

modules = {'requests': 'Network access ready',
           'pandas': 'Data manipulation ready',
           'matplotlib': 'Visualization ready',
           'numpy': 'Matrix ready'}


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
        data = {"x": modules['numpy'].array([1, 2, 3]), "y": modules['numpy'].array([4, 5, 6])}
        data['x'] *= 2
        data['y'] *= 2
        out = modules['pandas'].DataFrame(data)
        out.plot(x='x', y='y', kind='line')
        modules['matplotlib'].pyplot.savefig("plot.png", dpi=300)
    else:
        print('<Warming> You are not in virtual enviroment !')


if __name__ == "__main__":
    main()
