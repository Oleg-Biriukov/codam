from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr, Field
from typing import ClassVar
from hub import Hub


class Connection(ABC, BaseModel):
    __conct: ClassVar[dict[Hub, Hub]] = {}
    max_link_capacity: int = Field(gt=0, le=50, default=1)
