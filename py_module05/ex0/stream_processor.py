from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    @abstractmethod
    def process(self, data: Any) -> str:
        pass

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    def format_output(self, result: str) -> str:
        return f"Result: {result}"


class NumericProcessor(DataProcessor):
    @classmethod
    def process(self, data) -> str:
        try:
            return f"Processed {len(data)} numeric values,\
 sum={sum(data)}, avg={sum(data)/len(data)}"
        except Exception:
            return "ERROR: Connection timeout"

    @classmethod
    def validate(self, data: Any) -> bool:
        if type(data) is list:
            for i in data:
                if type(i) is int:
                    return True
                else:
                    return False
        else:
            return False


class TextProcessor(DataProcessor):
    @classmethod
    def process(self, data) -> str:
        try:
            return f"{len(data)} characters, {len(data.split(' '))} words"
        except Exception:
            return "ERROR: Connection timeout"

    @classmethod
    def validate(self, data) -> bool:
        if type(data) is str:
            return True
        else:
            return False


class LogProcessor(DataProcessor):
    @classmethod
    def process(self, data) -> str:
        try:
            
        except Exception:
            return "ERROR: Connection timeout"

    @classmethod
    def validate(self, data) -> bool:
        if type(data) is str:
            if data 
