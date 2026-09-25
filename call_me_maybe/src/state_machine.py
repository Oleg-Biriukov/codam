from pydantic import BaseModel
from enum import Enum, auto
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt
from src.definer_func import tp, Types
from src.file_edit import get_prompt
import re
import json


class State(Enum):
    PICK_STRING = auto()
    PICK_OTHERS = auto()
    PICK_NAME = auto()
    PICK_FUNC = auto()


class StateMachine(BaseModel):
    _f_tokens: list = 0
    _state: State = State.PICK_FUNC
    _func: FuncDefiner = None
    _is_done: bool = False
    _pointer: int = 0
    _is_was_qt: bool = True
    _i: int = 0
    _allowed_tkn = ['}', '"', '"}', ', ', ',', ' ']

    def get_pattern(self, _llm: any) -> any:
        name: list = [(n, t["type"]) for n, t in self._func.parameters.items()]
        pattern: list = ['']

        pattern[-1] += '{'
        for name, type in name:
            if type is Types.STRING:
                pattern[-1] += f'"{name}": "'
            else:
                pattern[-1] += f'"{name}": '
            # pattern.append(tpe)
            pattern.append('')
        pattern = [_llm.encode(txt).tolist()[0] for txt in pattern]
        return pattern

    def get_allowed_prmt_tokens(self, _llm: any, prompt: str) -> any:
        tokens = [tkn for txt in self._allowed_tkn
                  for tkn in _llm.encode(txt).tolist()[0]]
        return _llm.encode(prompt).tolist()[0] + tokens

    def get_allowed_func_tokens(self, llm: any, func: list) -> list:
        self._f_tokens = [llm.encode(fn.name).tolist()[0] for fn in func]
        return [t for l_t in self._f_tokens for t in l_t]

    def pick_token(self, llm: any, prompt: list, allowed_tokens: list = None):
        logits: list = llm.get_logits_from_input_ids(prompt)
        if self._state is State.PICK_FUNC:
            return max(allowed_tokens, key=lambda tkn: logits[tkn])
        return sorted(list(enumerate(logits)),
                      key=lambda tkn: tkn[1],
                      reverse=True)

    def is_ok(self, tkn_str: str, type: Types) -> bool:
        for ltr in tkn_str:
            if self._is_was_qt and (ltr == ',' or ltr == '}'):
                self._state = State.PICK_NAME
                break
            if ltr == '"' and self._state is State.PICK_STRING:
                self._is_was_qt = True
                continue
            if not re.fullmatch(tp[type], ltr):
                return False
        return True


class MiddlePerson(BaseModel):
    _llm: Small_LLM_Model = Small_LLM_Model()
    _stop_id: list
    _func: FuncDefiner
    _s_m: StateMachine
    _types: list
    user_prompt: list[UserPrompt]
    functions: list[FuncDefiner]
    _output: list = []

    def _get_func(self, fn_name: str) -> FuncDefiner:
        func: FuncDefiner

        for fn in self.functions:
            if fn.name == fn_name:
                func = fn
                break
        return func

    def _func_gen(self, prompt: list[int], allowed_tkn: list) -> None:
        token: int
        res: list = []

        while res not in self._s_m._f_tokens:
            token = self._s_m.pick_token(self._llm, prompt, allowed_tkn)
            print(self._llm.decode(token), end='', flush=True)
            prompt.append(token)
            res.append(token)
        self._func = self._get_func(self._llm.decode(res))
        self._s_m._func = self._func
        self._s_m._state = State.PICK_NAME

    def _prmt_gen(self, prompt: list[int]) -> None:
        token: int
        tkn_text: str
        pattern: list = self._s_m.get_pattern(self._llm)[::-1]

        while self._types:
            if self._s_m._state is State.PICK_NAME:
                prompt += pattern[-1]
                print(self._llm.decode(pattern.pop()), end='', flush=True)
                if self._types is Types.STRING:
                    self._s_m._state = State.PICK_STRING
                    self._is_was_qt = False
                else:
                    self._s_m._state = State.PICK_OTHERS
            tkn_text = ''
            logits = self._s_m.pick_token(self._llm, prompt)
            for token, _ in logits:
                tkn_text = self._llm.decode(token)
                if self._s_m.is_ok(tkn_text, self._types[0]):
                    break
            print(tkn_text, end='', flush=True)
            prompt.append(token)
            if self._s_m._state is State.PICK_NAME:
                self._types.pop(0)

    def get_out(self) -> list:
        self._output = [json.loads(resp) for resp in self._output]
        return self._output

    def gen_text(self) -> None:
        allowed_func_tkn: list
        prompt: list
        res: list

        self._s_m = StateMachine(llm=self._llm)
        allowed_func_tkn = self._s_m.get_allowed_func_tokens(self._llm,
                                                             self.functions)
        for prompt in self.user_prompt:
            llmprmt = self._llm.encode(get_prompt(prompt.prompt,
                                                  self.functions)).tolist()[0]
            res = self._llm.encode(
                '{' + f'"prompt": "{prompt.prompt}", "name": "'
                                   ).tolist()[0]
            print('{' + f'"prompt": "{prompt.prompt}", "name": "',
                  end='',
                  flush=True)
            self._func_gen(llmprmt+res, allowed_func_tkn)
            res += self._llm.encode('", ').tolist()[0]
            print('", ', end='', flush=True)
            self._types = [t["type"] for _, t in self._func.parameters.items()]
            print('"parameters": ', end='', flush=True)
            res += self._llm.encode("parameters: ").tolist()[0]
            self._prmt_gen(llmprmt+res)
            self._s_m._state = State.PICK_FUNC
            self._output.append(self._llm.decode(res))
            print(flush=True)
