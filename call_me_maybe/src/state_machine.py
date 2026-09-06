from pydantic import BaseModel
from enum import Enum, auto
import numpy as np
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt, Output
from src.file_edit import get_prmt_prompt, get_func_prompt
import functools as f
import re


class State(Enum):
    EXPECT_CBRC = auto()
    EXPECT_FUNC = auto()
    EXPECT_PRMTR = auto()
    DONE = auto()


class StateMachine(BaseModel):
    _llm: Small_LLM_Model = Small_LLM_Model()
    _state: State = State.EXPECT_FUNC
    _stop_id: list
    _res: list[int] = []
    user_prompt: UserPrompt
    functions: list[FuncDefiner]
    output: Output = Output()

    def _func_gen(self, prompt: list[int]) -> None:
        token: int
        logits: list
        # made for begng state to determine the start point (take quarter of name and split it into letters )
        f_tokens: list = [self._llm.encode(fn.name).tolist()[0] for fn in self.functions]
        all_possible_tokens: list = [t for l_t in f_tokens for t in l_t]

        if self._state is State.EXPECT_PRMTR:
            self.output.name = self._llm.decode(self._res)
            return
        logits = np.array(self._llm.get_logits_from_input_ids(prompt))
        token = int(np.argmax(logits))
        # print(text_token)
        # print(all_possible_tokens, f_tokens)
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
        # return self._func_gen(prompt)

    def _prmt_gen(self):
        pass
    
    def gen_text(self):
        extractor_prmt: list
        extractor_func: list

        extractor_prmt = self._llm.encode(get_prmt_prompt(self.user_prompt, self.functions)).tolist()[0]
        extractor_func = self._llm.encode(get_func_prompt(self.user_prompt.prompt, self.functions)).tolist()[0]
        self._stop_id = ['}', '"}', ' "}', '"', ' ', '\n', '\r']
        if self._state is State.EXPECT_FUNC:
            self._func_gen(extractor_func)
            
