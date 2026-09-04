from pydantic import BaseModel
from enum import Enum, auto
import numpy as np
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt, Output
from src.file_edit import get_extr_prompt, get_func_prompt
import functools as f
import re


class State(Enum):
    EXPECT_CBRC = auto()
    EXPECT_QUOTE = auto()
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

        if self._state is State.EXPECT_PRMTR:
            self.output.name = self._llm.decode(self._res)
            return
        logits = np.array(self._llm.get_logits_from_input_ids(prompt))
        if self._state is State.EXPECT_FUNC:
            token = int(np.argmax(logits))
            if list(set(self._llm.decode([token])) & set(self._stop_id)): # probably problem somewhere here but im not sure
                self._state = State.EXPECT_PRMTR
            elif re.fullmatch('^[a-zA-Z_"]$', self._llm.decode(token)) and ' ' not in self._llm.decode(token):
                self._state = State.EXPECT_FUNC
            else:
                logits[token] = float("-inf")
        if self._state is State.EXPECT_QUOTE:
            while self._state is State.EXPECT_QUOTE:
                token = int(np.argmax(logits))
                if '"' in self._llm.decode(token)[0] and ' ' not in self._llm.decode(token):
                    self._state = State.EXPECT_FUNC
                else:
                    logits[token] = float("-inf")
        print(self._llm.decode(token), end='', flush=True)
        prompt.append(token)
        self._res.append(token)
        return self._func_gen(prompt)

    def _prmt_gen(self):
        pass
    
    def gen_text(self):
        extractor_prmt: list
        extractor_func: list

        # extractor_prmt = self._llm.encode(get_extr_prompt(self.user_prompt, self.functions)).tolist()[0]
        extractor_func = self._llm.encode(get_func_prompt(self.user_prompt.prompt, self.functions)).tolist()[0]
        self._stop_id = ['}', '"}', ' "}', '"']
        if self._state is State.EXPECT_FUNC:
            self._state = State.EXPECT_QUOTE
            self._func_gen(extractor_func)
            print(self._res)
            # print(self._llm.decode(self._res))
            
