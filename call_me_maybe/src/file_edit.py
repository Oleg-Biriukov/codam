from src.definer_func import FuncDefiner

def _print_prmt(func: FuncDefiner) -> str:
    prmt_l: str = ""
    len_prmtr: int

    len_prmtr = len(func.parameters)
    for name, type in func.parameters.items():
        if len_prmtr > 1:
            prmt_l += f'{name}: {type['type'].value}, '
        else:
            prmt_l += f'{name}: {type['type'].value}'
        len_prmtr -= 1
    return prmt_l


def get_prmt_prompt(user_prompt: str, functs: list[FuncDefiner]) -> callable:
    extr_prompt: str = ""
    rtn: callable

    with open("llm_sdk/extractor_prompt.txt", "r") as prm:
        extr_prompt += prm.read()

    rtn = lambda func_name: f'''{extr_prompt}

User: "{user_prompt}"
Selected function: {func_name.name}
Available parameters: {_print_prmt(func_name)}
Correct: '''
    return rtn

def get_func_prompt(user_prompt: str, functs: list[FuncDefiner]) -> str:
    extr_prompt: str = ""

    with open("llm_sdk/func_definer.txt", "r") as func:
        extr_prompt += func.read()

    extr_prompt += f'User: "{user_prompt}"\nAvailable functions:\n'
    for func in functs:
        extr_prompt += f'- {func.name}: {func.description} ('
        extr_prompt += f'{_print_prmt(func)})\n'
    return extr_prompt + "Correct: "
