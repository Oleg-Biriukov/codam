from pydantic import BaseModel, model_validator
from enum import Enum, auto
import numpy as np
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt, Output
from src.definer_func import tp
from src.file_edit import get_prmt_prompt, get_func_prompt
import functools as f
import re


class State(Enum):
    DONE = auto()  # makr as done for response
    IN_PATTERN = auto()
    EXPECT_VALUE = ': '  # expect start value


class PrmtChecker(BaseModel):
    prompt: str
    func: FuncDefiner
    _pattern: str = ""
    _state: State = State.IN_PATTERN
    _pointer: int = 0
    _name_type: list = []
    _res: str = ""

    @model_validator(mode='after')
    def _constr_pattern(self) -> any:
        len_prmt: int = len(self.func.parameters)
        self._name_type = [(n, t_d["type"])
                           for n, t_d in self.func.parameters.items()]

        self._pattern += '{'
        for name, _ in self._name_type:
            self._pattern += f'"{name}": '
            if len_prmt != 1:
                self._pattern += ', '
            len_prmt -= 1
        self._pattern += '}'
        return self

    def is_ok(self, tkn_str: str) -> bool:
        back: int = 0
        if len(self._name_type):
            _, tpe = self._name_type[0]

        if self._state is State.DONE:
            return True
        for ltr in tkn_str:
            if self._res[-2:] == State.EXPECT_VALUE.value:
                self._state = State.EXPECT_VALUE

            if (self._pattern[self._pointer] != ltr and
                    self._state is State.IN_PATTERN):
                self._pointer -= back
                return False

            if ltr == ',' or ltr == '}' and len(self._name_type):
                self._name_type.pop(0)
                self._state = State.IN_PATTERN

            if self._state is State.EXPECT_VALUE:
                if re.fullmatch(tp[tpe], ltr) is None:
                    return False

            if ltr == '}' and self._pattern[self._pointer] == '}':
                self._state = State.DONE
                return True

            if self._state is State.IN_PATTERN:
                self._pointer += 1
                back += 1
            self._res += ltr
        return True


class StateMachine(BaseModel):
    _llm: Small_LLM_Model = Small_LLM_Model()
    _stop_id: list
    _res: list[int] = []
    _func: FuncDefiner
    user_prompt: UserPrompt
    functions: list[FuncDefiner]
    output: Output = Output()

    def _func_gen(self, prompt: list[int]) -> None:
        token: int
        logits: list
        # made for begng state to determine the start point (take quarter of name and split it into letters )
        f_tokens: list = [self._llm.encode(fn.name).tolist()[0] for fn in self.functions]
        all_possible_tokens: list = [t for l_t in f_tokens for t in l_t]

        logits = np.array(self._llm.get_logits_from_input_ids(prompt))
        token = int(np.argmax(logits))
        while self._res not in f_tokens:
            token = int(np.argmax(logits))
            if token in all_possible_tokens:
                print(self._llm.decode(token), end='', flush=True)
                prompt.append(token)
                self._res.append(token)
                logits = np.array(self._llm.get_logits_from_input_ids(prompt))
                token = int(np.argmax(logits))
            else:
                logits[token] = float("-inf")
        self._state = State.EXPECT_PRMTR
        self.output.name = self._llm.decode(self._res)

    def _prmt_gen(self, prompt: list[int]) -> None:
        token: int
        logits: list
        tkn_text: str
        prmtr_checker: PrmtChecker

        logits = np.array(self._llm.get_logits_from_input_ids(prompt))
        prmtr_checker = PrmtChecker(prompt=self.user_prompt.prompt,
                                    func=self._func)
        while prmtr_checker._state is not State.DONE:
            token = int(np.argmax(logits))
            tkn_text = self._llm.decode(token)
            if prmtr_checker.is_ok(tkn_text):
                print(self._llm.decode(token), end='', flush=True)
                prompt.append(token)
                self._res.append(token)
                logits = np.array(self._llm.get_logits_from_input_ids(prompt))
                token = int(np.argmax(logits))
            else:
                logits[token] = float("-inf")

    def gen_text(self):
        def get_func(fn_name: str) -> FuncDefiner:
            func: FuncDefiner

            for fn in self.functions:
                if fn.name == fn_name:
                    func = fn
                    break
            return func

        extractor_prmt: list
        extractor_func: list

        extractor_func = self._llm.encode(get_func_prompt(self.user_prompt.prompt, self.functions)).tolist()[0]
        self.output.name = "fn_substitute_string_with_regex"
        self._func = get_func(self.output.name)
        extractor_prmt = self._llm.encode(get_prmt_prompt(self.user_prompt,
                                                          self.functions)
                                          (self._func)).tolist()[0]
        # if self._state is State.EXPECT_FUNC:
        #     self._func_gen(extractor_func)
        #     extractor_prmt = self._llm.encode(get_prmt_prompt(self.user_prompt,
        #                                                       self.functions)
        #                                       (self.output.name)
        #                                       ).tolist()[0]
        self._prmt_gen(extractor_prmt)
        
