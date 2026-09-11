from pydantic import BaseModel
from enum import Enum, auto
import numpy as np
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt, Output
from src.definer_func import tp
from src.file_edit import get_prmt_prompt, get_func_prompt
import functools as f
import re


class State(Enum):
    EXPECT_O_CBRC = auto()  # expect open curl bracket
    EXPECT_C_CBRC = auto()  # expect close curl bracket
    EXPECT_O_QUOTE = auto()  # expect open quote
    EXPECT_FUNC = auto()  # expect function
    DONE = auto()  # makr as done for response
    EXPECT_C_QUOTE = [',', ':', ' ']  # expect open quote
    EXPECT_VALUE = ': '  # expect start value
    EXPECT_PRMTR = ', "'  # expect parameter field


class StateMachine(BaseModel):
    _llm: Small_LLM_Model = Small_LLM_Model()
    _state: State = State.EXPECT_FUNC
    _stop_id: list
    _res: list[int] = []
    _func: FuncDefiner
    _name_type: list = []
    user_prompt: UserPrompt
    functions: list[FuncDefiner]
    output: Output = Output()

    def _state_dfnr(self, text: str) -> bool:
        # print(self._state, text[0])
        if text == '':
            return True

        elif self._state is State.EXPECT_O_CBRC:
            if '{' != text[0]:
                return False
            self._state = State.EXPECT_O_QUOTE

        elif self._state is State.EXPECT_C_CBRC:
            if text[0] != '}':
                return False
            self._state = State.DONE

        elif self._state is State.EXPECT_O_QUOTE:
            if '"' != text[0]:
                return False
            self._state = State.EXPECT_PRMTR

        elif self._state is State.EXPECT_VALUE:
            _, tp_ = self._name_type[0]

            if text[:2] == ', ':
                self._name_type.pop(0)
                self._state = State.EXPECT_O_QUOTE
                return self._state_dfnr(text[2:])
            if text[0] == '}':
                self._state = State.EXPECT_C_CBRC
                return self._state_dfnr(text)
            if not re.fullmatch(tp[tp_], text[0]):
                return False

        elif self._state is State.EXPECT_PRMTR:
            name, _ = self._name_type[0]

            if text[0] == '"':
                self._state = State.EXPECT_C_QUOTE
            elif not (set(list(text)) & set(list(name))):
                return False

        elif self._state is State.EXPECT_C_QUOTE:
            if set(text[:2]) & set(State.EXPECT_VALUE.value):
                text = text[1:]
                self._state = State.EXPECT_VALUE
            elif set(text[:3]) & set(State.EXPECT_PRMTR.value):
                text = text[2:]
                self._state = State.EXPECT_PRMTR
            elif text[0] not in State.EXPECT_C_QUOTE.value:
                return False
            if text[0] == '}':
                self._state = State.EXPECT_C_CBRC
                return self._state_dfnr(text)

        return self._state_dfnr(text[1:])

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

    def _prmt_gen(self, prompt: list[int]):
        token: int
        logits: list
        tkn_text: str
        # made for begng state to determine the start point (take quarter of name and split it into letters )
        f_tokens: list = [self._llm.encode(fn.name).tolist()[0] for fn in self.functions]
        all_possible_tokens: list = [t for l_t in f_tokens for t in l_t]

        if self._state is State.EXPECT_PRMTR:
            self.output.name = self._llm.decode(self._res)
            return
        logits = np.array(self._llm.get_logits_from_input_ids(prompt))
        token = int(np.argmax(logits))
        tkn_text = self._llm.decode(token)
        self._state = State.EXPECT_O_CBRC
        while self._state is not State.DONE:
            token = int(np.argmax(logits))
            # if token in all_possible_tokens:
            print(self._llm.decode(token), end='', flush=True)
            prompt.append(token)
            self._res.append(token)
            logits = np.array(self._llm.get_logits_from_input_ids(prompt))
            token = int(np.argmax(logits))
            # else:
            #     logits[token] = float("-inf")

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

        # extractor_func = self._llm.encode(get_func_prompt(self.user_prompt.prompt, self.functions)).tolist()[0]
        self.output.name = "fn_add_numbers"
        self._func = get_func(self.output.name)
        extractor_prmt = self._llm.encode(get_prmt_prompt(self.user_prompt,
                                                          self.functions)
                                          (self._func)).tolist()[0]
        self._state = State.EXPECT_O_CBRC
        self._name_type = [(n, t_d["type"])
                           for n, t_d in self._func.parameters.items()]
        print(self._state_dfnr('{"a": 40,'))
        print(self._state_dfnr(' "b": 30}'))
        
        # if self._state is State.EXPECT_FUNC:
        #     self._func_gen(extractor_func)
        #     extractor_prmt = self._llm.encode(get_prmt_prompt(self.user_prompt,
        #                                                       self.functions)
        #                                       (self.output.name)
        #                                       ).tolist()[0]
        # self._prmt_gen(extractor_prmt)
        
