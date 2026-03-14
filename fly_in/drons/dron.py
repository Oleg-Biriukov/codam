from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr, Field
from enum import Enum


class Dron(BaseModel):
    id: int = Field(ge=0, le=10000)
    pos: tuple[int, int]
