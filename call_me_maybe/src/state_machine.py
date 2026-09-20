from pydantic import BaseModel, model_validator
from enum import Enum, auto
import numpy as np
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt, Output
from src.definer_func import tp, Types
from src.file_edit import get_prmt_prompt, get_func_prompt
import functools as f
import re
import json


class StateMachine(BaseModel):
    func: FuncDefiner = None
    state: State = State.
    _f_tokens: list = 0
    _pattern: list = ['']
    _is_done: bool = False
    _pointer: int = 0
    _i: int = 0

    def _constr_pattern(self) -> any:
        len_prmt: int = len(self.func.parameters)
        name_type: list = [(n, t_d["type"])
                           for n, t_d in self.func.parameters.items()]

        if self.func is None:
            return
        self._pattern[-1] += '{'
        for name, _ in name_type:
            self._pattern[-1] += f'"{name}": '
            # self._pattern.append(tpe)
            self._pattern.append('')
            if len_prmt != 1:
                self._pattern[-1] += ' '
            len_prmt -= 1
        self._pattern[-1] += '}'

    def get_allowed_func_tokens(self, llm: any, func: list) -> list:
        self._f_tokens = [llm.encode(fn.name).tolist()[0] for fn in func]
        return [t for l_t in self._f_tokens for t in l_t]

    def get_allowed_prmt_tokens(self, llm: any) -> list:
        if self._pattern == ['']:
            return []
        f_tokens: list = [llm.encode(txt).tolist()[0]
                          for txt in self._pattern]
        return [t for l_t in f_tokens for t in l_t]

    def pick_token(self, llm: any, prompt: list, allowed_tokens: list):
        logits = llm.get_logits_from_input_ids(prompt)
        return max(allowed_tokens, key=lambda tkn: logits[tkn])

    def is_ok(self, tkn_str: str) -> bool:
        pointer: any
        back: int = self._i
        back_pattern: list = self._pattern

        for ltr in tkn_str:
            if len(self._pattern):
                pointer = back_pattern[0]
            else:
                self._is_done = True
                return False

            if isinstance(pointer, str):
                if ltr != pointer[self._i]:
                    self._i = back
                    return False
                elif self._i + 1 == len(pointer):
                    back_pattern.pop(0)
                    self._i = 0
                    back = 0
                else:
                    back += 0
                    self._i += 1
            if isinstance(pointer, Types):
                if ltr == ',' or ltr == '}':
                    back_pattern.pop(0)
                elif re.fullmatch(tp[pointer], ltr) is None:
                    self._i = back
                    return False
            self._pattern = back_pattern
        return True


class MiddlePerson(BaseModel):
    _llm: Small_LLM_Model = Small_LLM_Model()
    _stop_id: list
    _func: FuncDefiner
    _s_m: StateMachine
    user_prompt: list[UserPrompt]
    functions: list[FuncDefiner]
    output: Output = Output()

    def _softmax(self, logits: np.array) -> np.array:
        exp = np.exp(logits - np.max(logits))
        return exp / np.sum(exp)

    def _func_gen(self, prompt: list[int], allowed_tkn: list) -> None:
        token: int
        res: list = []

        while res not in self._s_m._f_tokens:
            token = self._s_m.pick_token(self._llm, prompt, allowed_tkn)
            print(self._llm.decode(token), end='', flush=True)
            prompt.append(token)
            res.append(token)
        self.output.name = self._llm.decode(res)

    def _prmt_gen(self, prompt: list[int], txt_prompt: str) -> None:
        token: int
        logits: list
        tkn_text: str
        prmtr_checker: PrmtChecker

        logits = np.array(self._llm.get_logits_from_input_ids(prompt))
        prmtr_checker = PrmtChecker(prompt=txt_prompt,
                                    func=self._func)
        # print(prmtr_checker._pattern, flush=True)
        while not prmtr_checker._is_done:
            token = int(np.argmax(logits))
            # tkn_text = self._voc[token]
            tkn_text = self._llm.decode(token)
            if prmtr_checker.is_ok(tkn_text):
                print(tkn_text, end='', flush=True)
                prompt.append(token)
                self._res.append(token)
                logits = np.array(self._llm.get_logits_from_input_ids(prompt))
                token = int(np.argmax(logits))
            else:
                logits[token] = float("-inf")
        # print(prmtr_checker._state)
        print(flush=True)

    def gen_text(self):
        def get_func(fn_name: str) -> FuncDefiner:
            func: FuncDefiner

            for fn in self.functions:
                if fn.name == fn_name:
                    func = fn
                    break
            return func

        self._s_m = StateMachine(func=get_func("fn_add_numbers"),
                                 llm=self._llm)
        extractor_prmt: list
        extractor_func: list
        allowed_func_tkn = self._s_m.get_allowed_func_tokens(self._llm,
                                                             self.functions)

        # for prompt in self.user_prompt:
        prompt = self.user_prompt[3]
        res = self._llm.encode(get_func_prompt(prompt.prompt,
                                               self.functions)).tolist()[0]
        res += self._llm.encode('{' + f'"prompt": {prompt.prompt}, name: "').tolist()[0]
        print(self._llm.decode(res), end='', flush=True)
        self._func_gen(res, allowed_func_tkn)
            # self._func = get_func(self.output.name)
            # extractor_prmt = self._llm.encode(get_prmt_prompt(prompt.prompt,
            #                                                   self.functions)
            #                                   (self._func)).tolist()[0]
            # print('", parameters: ', end='', flush=True)
            # self._res = []
            # self._prmt_gen(extractor_prmt, prompt.prompt)
            # self._res = []
        print(flush=True)
        
