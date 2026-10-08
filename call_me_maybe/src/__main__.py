import json
import argparse
from src.definer_func import FuncDefiner, UserPrompt, Types
from src.state_machine import MiddlePerson
from src.error_handler import error_handler
import os
import time


@error_handler
def take_out_arg() -> argparse.Namespace:
    """
    Function for parcing the arguments and preparing field for executing
    """
    parser: argparse.ArgumentParser
    arg: argparse.Namespace
    out_dir: str = "data/output"

    parser = argparse.ArgumentParser(
        prog='call_me_maybe',
        description='The function calling system.')
    parser.add_argument("--functions_definition", type=str,
                        default="data/input/functions_definition.json")
    parser.add_argument("--input", type=str,
                        default="data/input/function_calling_tests.json")
    if not os.path.isdir("data/output/"):
        os.mkdir("data/output")
    parser.add_argument("--output", type=str,
                        default="data/output/output.json")
    arg = parser.parse_args()
    if not os.path.isdir(out_dir) and out_dir in arg.output:
        os.mkdir(out_dir)
    if not os.path.isdir('/'.join(arg.output.split('/')[:-1])):
        raise FileExistsError("output")
    return arg


@error_handler
def take_out_func_from_file(filename: str) -> list:
    """
    Parcing function and store them into FuncDefiner Class.
    """
    func_list: list = []
    content: list
    dscpt: str = "In case if you cannot find needed function"

    with open(filename, "r") as func_file:
        content = json.load(func_file)
    for func in content:
        func_list.append(FuncDefiner(**func))
    func_list.append(FuncDefiner(name="None",
                                 description=dscpt,
                                 parameters={},
                                 returns={'type': Types.NONE}))
    return func_list


@error_handler
def get_user_prompt_from_file(filename: str) -> list:
    """
    Read prompts and let them trow UserPrompt validator.
    """
    users_prompt: list = []
    content: list

    with open(filename, "r") as userprompt:
        content = json.load(userprompt)
    for prompt in content:
        users_prompt.append(UserPrompt(**prompt))
    return users_prompt


@error_handler
def load_response_to_file(filename: str, response: list) -> None:
    """
    Save responses in file
    """
    for r in response:
        r['name'] = r['name'].strip()
        if r['parameters'] is None:
            continue
        for name, value in r['parameters'].items():
            if isinstance(r['parameters'][name], str):
                if r['parameters'][name] != ' ':
                    r['parameters'][name] = value.strip()
                if r['parameters'][name] == "string":
                    r['parameters'][name] = ""

    with open(filename, "w") as output:
        json.dump(response, output, indent=4)


def main() -> None:
    arg: argparse.Namespace
    function_list: list[FuncDefiner]
    users_prompt: list[UserPrompt]

    arg = take_out_arg()
    function_list = take_out_func_from_file(arg.functions_definition)
    users_prompt = get_user_prompt_from_file(arg.input)
    state_machine = MiddlePerson(user_prompt=users_prompt,
                                 functions=function_list)
    start = time.perf_counter()
    state_machine.gen_text()
    end = time.perf_counter()
    print(f'LLM completed in {(end-start)/60:.6f} min')
    load_response_to_file(arg.output, state_machine.get_out())


if __name__ == "__main__":
    main()
