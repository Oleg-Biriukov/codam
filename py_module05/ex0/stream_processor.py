from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    @abstractmethod
    def process(self, data: Any) -> str:
        pass

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @classmethod
    def format_output(cls, result: str) -> str:
        return f"Output: {result}"


class NumericProcessor(DataProcessor):
    @classmethod
    def process(cls, data) -> str:
        try:
            return f"Processed {len(data)} numeric values,\
 sum={sum(data)}, avg={sum(data)/len(data)}"
        except Exception:
            return "ERROR: Connection timeout"

    @classmethod
    def validate(cls, data: Any) -> bool:
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
    def process(cls, data) -> str:
        try:
            return f"{len(data)} characters, {len(data.split(' '))} words"
        except Exception:
            return "ERROR: Connection timeout"

    @classmethod
    def validate(cls, data) -> bool:
        if type(data) is str:
            return True
        else:
            return False


class LogProcessor(DataProcessor):
    @classmethod
    def process(cls, data) -> str:
        return data

    @classmethod
    def validate(cls, data) -> bool:
        log_headers = ['[ERROR]', '[STATUS]', '[INFO]']
        if type(data) is str:
            for log_h in log_headers:
                if log_h in data:
                    return True
            return False

    @classmethod
    def format_output(cls, result):
        return f"Error was catched => {result}"


if __name__ == "__main__":
    lst = [1, 2, 3, 4, 5]
    print("Initializing Numeric Processor...")
    print("Processing data:", lst)
    if NumericProcessor.validate([1, 2, 3, 4, 5]):
        print("Validation: Numeric data verified")
        result = NumericProcessor.format_output(NumericProcessor.process(lst))
        print(result)
    else:
        print("Error: Numeric data unverified")

    text = "Hello Nexus World"
    print("\nInitializing Text Processor...")
    print("Processing data:", text)
    if TextProcessor.validate(text):
        print("Validation: Text data verified")
        result = TextProcessor.format_output(TextProcessor.process(text))
        print(result)
    else:
        print("Error: Text data unverified")

    log = '[ERROR] invalid data'
    print("\nInitializing Log Processor...")
    print("Processing data:", log)
    if LogProcessor.validate(log):
        print("Validation: Log data verified")
        result = LogProcessor.format_output(LogProcessor.process(log))
        print(result)
    else:
        print("Error: Log data unverified")
