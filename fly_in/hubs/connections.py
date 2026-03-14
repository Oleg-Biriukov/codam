from abc import ABC, abstractmethod
from pydantic import BaseModel, PrivateAttr, Field
from typing import ClassVar
from hub import Hub


class Connection(ABC, BaseModel):
    __conct: ClassVar[dict[Hub, Hub]] = {}
    
    
