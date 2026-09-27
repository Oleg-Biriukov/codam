from src.definer_func import FuncDefiner


def _print_prmt(func: FuncDefiner) -> str:
    prmt_l: str = ""
    len_prmtr: int

    len_prmtr = len(func.parameters)
    for name, type in func.parameters.items():
        if len_prmtr > 1:
            prmt_l += f"{name}: {type['type'].value}, "
        else:
            prmt_l += f"{name}: {type['type'].value}"
        len_prmtr -= 1
    return prmt_l


def get_prmt_prompt(user_prompt: str, func: FuncDefiner, res: str) -> str:
    extr_prompt: str = ""

    with open("llm_sdk/prmt_prompt.txt", "r") as f:
        extr_prompt += f.read()

    extr_prompt = f'''<|im_start|>system
{extr_prompt}
<|im_end|>
<|im_start|>user
{user_prompt}
Selected function: {func.name} ({_print_prmt(func)})\n'''
    return extr_prompt + f"<|im_end|>\n<|im_start|>assistant\nAnswer: {res}"


def get_func_prompt(user_prompt: str, functs: list[FuncDefiner]) -> str:
    extr_prompt: str = ""

    with open("llm_sdk/func_prompt.txt", "r") as func:
        extr_prompt += func.read()

    extr_prompt = f'''<|im_start|>system
{extr_prompt}
<|im_end|>
<|im_start|>user
{user_prompt}
Available functions:\n'''
    for func in functs:
        extr_prompt += f'"{func.name}":{func.description}({_print_prmt(func)})\n'
    return extr_prompt + "<|im_end|>\n<|im_start|>assistant\nAnswer: "
