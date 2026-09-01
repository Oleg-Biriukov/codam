from llm_sdk.llm_sdk import Small_LLM_Model
import json
import argparse
from src.definer_func import FuncDefiner
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
    return func_list


def main():
    model: Small_LLM_Model
    arg: argparse.Namespace
    function_list: list[FuncDefiner]

    # model = Small_LLM_Model()
    arg = take_out_arg()
    function_list = take_out_func_from_file(arg.functions_definition)
    # while 1:
    #     logits = enumerate(model.get_logits_from_input_ids(prompt))
    #     logits = sorted(logits, key=lambda x: x[1], reverse=True)
    #     prompt.append(logits[0][0])
    #     print(model.decode([logits[0][0]]), end='', flush=True)
    #     if logits[0][0] == 151645:
    #         break
if __name__ == "__main__":
    main()
