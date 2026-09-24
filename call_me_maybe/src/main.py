import json
import argparse
from src.definer_func import FuncDefiner, UserPrompt
from src.state_machine import MiddlePerson
from src.file_edit import get_prmt_prompt, get_func_prompt
import os
import time


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
    state_machine = MiddlePerson(user_prompt=users_prompt,
                                 functions=function_list)
    # print(get_func_prompt(users_prompt[0].prompt, function_list))
# ===========================test===========================
    # import numpy as np
    # print(state_machine._llm.get_path_to_vocab_file())
    # with open(state_machine._llm.get_path_to_vocab_file(), "r") as v:
    #     _voc = json.load(v)
    #     _voc = {int(idx): item for item, idx in _voc.items()}

    # def softmax(logits: np.array):
    #     exp = np.exp(logits - np.max(logits))
    #     return exp / np.sum(exp)
    # req = state_machine._llm.encode(get_func_prompt(users_prompt, function_list)).tolist()[0]
    # logits = np.array(state_machine._llm.get_logits_from_input_ids(req))
    # print(len(logits), len(_voc))
    # # logits = softmax(logits)
    # for _ in range(10):
    #     lg = int(np.argmax(logits))
    #     print(state_machine._llm.decode(lg))
    #     logits[lg] = float("-inf")
# ==========================================================
    start = time.perf_counter()
    state_machine.gen_text()
    end = time.perf_counter()
    print(f'LLM completed in {(end-start)/60:.6f} min')


if __name__ == "__main__":
    main()
