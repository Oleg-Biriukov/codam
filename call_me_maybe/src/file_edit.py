from src.definer_func import FuncDefiner


def get_extr_prompt(user_prompt: str, functs: list[FuncDefiner]) -> str:
    extr_prompt: str = "\tList of Func\n"

    for func in functs:
        extr_prompt += f"Name: {func.name}\n"
        extr_prompt += f"Description: {func.description}\n"
        extr_prompt += "Parameters:\n"
        for var, type in func.parameters.items():
            extr_prompt += f"\tName: {var}\n"
            extr_prompt += f"\tType: {type["type"].value}\n\n"
        extr_prompt += f"Return Type: {func.returns["type"].value}\n---\n"

    extr_prompt += '===\n\n'
    
    with open("llm_sdk/extractor_prompt.txt", "r") as prm:
        extr_prompt += prm.read()

    extr_prompt += '\n===\n'

    return extr_prompt + "Prompt: " + user_prompt

def get_func_prompt(user_prompt: str, functs: list[FuncDefiner]) -> str:
    extr_prompt: str = ""
    with open("llm_sdk/func_definer.txt", "r") as func:
        extr_prompt += func.read()

    extr_prompt += f'User: "{user_prompt}"\nAvailable functions:\n'
    for func in functs:
        extr_prompt += f"- {func.name}: {func.description}\n"
    return extr_prompt + "Correct: "
