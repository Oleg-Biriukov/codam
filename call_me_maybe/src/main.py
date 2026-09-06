import json
import argparse
from src.definer_func import FuncDefiner, UserPrompt
from src.state_machine import StateMachine
from src.file_edit import get_prmt_prompt, get_func_prompt
import os


def take_out_arg() -> argparse.Namespace:
    parser: argparse.ArgumentParser

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
    return parser.parse_args()


def take_out_func_from_file(filename: str) -> list[FuncDefiner]:
    func_list: list[FuncDefiner] = []
    content: list[dict]

    with open(filename, "r") as func_file:
        content = json.load(func_file)
    for func in content:
        func_list.append(FuncDefiner(**func))
    func_list.append(FuncDefiner(name="None",
                                 description="In case if you cannot find needed function",
                                 parameters={},
                                 returns={'type': 'None'}))
    return func_list


def get_user_prompt_from_file(filename: str) -> list[UserPrompt]:
    users_prompt: list[FuncDefiner] = []
    content: list[dict]

    with open(filename, "r") as userprompt:
        content = json.load(userprompt)
    for prompt in content:
        users_prompt.append(UserPrompt(**prompt))
    return users_prompt


def main():
    arg: argparse.Namespace
    function_list: list[FuncDefiner]
    users_prompt: list[UserPrompt]

    arg = take_out_arg()
    function_list = take_out_func_from_file(arg.functions_definition)
    users_prompt = get_user_prompt_from_file(arg.input)
    print(get_prmt_prompt(users_prompt[0].prompt, function_list)(function_list[0].name), flush=True)
    # state_machine = StateMachine(user_prompt=users_prompt[2],
    #                              functions=function_list)
    # state_machine.gen_text()
    # print(state_machine.output.name)
    
    # while 1:
    #     
    #     prompt.append(logits[0][0])
    #     print(model.decode([logits[0][0]]), end='', flush=True)
    #     if logits[0][0] == 151645:
    #         break


if __name__ == "__main__":
    main()
