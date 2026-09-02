from pydantic import BaseModel, model_validator, Field
from enum import Enum


class Types(Enum):
    STRING = 'string'
    NUM = 'number'


class FuncDefiner(BaseModel):
    name: str = Field(min_length=2, max_length=50)
    description: str = Field(min_length=2, max_length=500)
    parameters: dict[str, dict[str, Types]]
    returns: dict[str, Types]

    @model_validator(mode="after")
    def check_correctness(self):
        if len(self.returns.keys()) > 1 or "type" not in self.returns.keys():
            raise ValueError("Inappropriate return type.")
        for _, tp_prmtr in self.parameters.items():
            if (len(tp_prmtr.keys()) > 1
                    or "type" not in tp_prmtr.keys()
                    or tp_prmtr["type"] not in Types):
                raise ValueError("Inappropriate return type.")
        return self


class UserPrompt(BaseModel):
    prompt: str = Field(min_length=2, max_length=150)
