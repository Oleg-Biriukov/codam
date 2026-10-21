from pydantic import BaseModel, model_validator, Field, ConfigDict
from enum import Enum
from typing import Any
import re


class Types(Enum):
    STRING = 'string'
    INTEGER = 'integer'
    NUM = "number"
    LIST = "list"
    BOOL = "boolean"
    NONE = "None"


tp: dict = {
    Types.STRING: r"^[a-zA-Z0-9_\-*+/\\'\" .:,{}()]*$",
    Types.NUM: r"^[0-9-.]*$",
    Types.INTEGER: r"^[0-9-]*$",
    Types.LIST: r"^.*$",
    Types.BOOL: r"^(True|False)$",
    Types.NONE: r"^None$"
}


class FuncDefiner(BaseModel):
    """Class that stores all needed field and rules for that
    for receiving func"""
    model_config = ConfigDict(extra='forbid')
    name: str = Field(pattern="^[a-zA-Z_]{2,50}$")
    description: str = Field(min_length=2, max_length=500)
    parameters: dict[str, dict[str, Types]]
    returns: dict[str, Types]

    @model_validator(mode="after")
    def check_correctness(self) -> Any:
        if len(self.returns.keys()) > 1 or "type" not in self.returns.keys():
            raise ValueError("Inappropriate return type.")
        for n, tp_prmtr in self.parameters.items():
            if not re.fullmatch("^[a-zA-Z_]{0,50}$", n):
                raise ValueError(f"Inappropriate name of the parameter.({n})")
            if (len(tp_prmtr.keys()) > 0):
                if ("type" not in tp_prmtr.keys()
                        or tp_prmtr["type"] not in Types):
                    raise ValueError("Inappropriate parameter property.")
        return self


class UserPrompt(BaseModel):
    """Class that stores all needed field and rules for that
        for receiving prompt"""
    model_config = ConfigDict(extra='forbid')
    prompt: str = Field(min_length=1, max_length=300)
