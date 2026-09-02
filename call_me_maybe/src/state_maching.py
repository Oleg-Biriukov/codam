from pydantic import BaseModel, model_validator, Field
from enum import Enum
from llm_sdk.llm_sdk import Small_LLM_Model
from src.definer_func import FuncDefiner, UserPrompt


class State(Enum):
    pass


class StateMachine(BaseModel):
    _model: Small_LLM_Model = Small_LLM_Model()
    state: State
    user_prompt: UserPrompt
    functions: FuncDefiner

    def _proccess_greedy_pick(self, tokens: list) -> list:
        logits: list

        logits = enumerate(self._model.get_logits_from_input_ids(tokens))
        logits = sorted(logits, key=lambda x: x[1], reverse=True)
        tokens.append(logits[0][0])
        return tokens

    def launch(self):
        extractor_prmt: list
        func_definer: list

        extractor_prmt = self._model.encode(get_extr_prompt()).tolist()[0]
        func_definer = self._model.encode(get_dfn_prompt()).tolist()[0]
