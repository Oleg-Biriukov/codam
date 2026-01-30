from abc import ABC, abstractmethod
from typing import Any


class DataProcessor(ABC):
    @abstractmethod
    def process(self, data: Any) -> str:
        pass

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def format_output(self, result: str) -> str:
        return f"Output: {result}"


class NumericProcessor(DataProcessor):
    def process(self, data) -> str:
        try:
            return f"Processed {len(data)} numeric values,\
 sum={sum(data)}, avg={sum(data)/len(data)}"
        except Exception:
            return "ERROR: Connection timeout"

    def validate(self, data: Any) -> bool:
        if type(data) is list:
            for i in data:
                if type(i) is int:
                    return True
                else:
                    return False
        else:
            return False

    def format_output(self, result) -> str:
        out = super().format_output(result)
        return out + "\nNumericProcessor done successfully"


class TextProcessor(DataProcessor):
    def process(self, data) -> str:
        try:
            return f"{len(data)} characters, {len(data.split(' '))} words"
        except Exception:
            return "ERROR: Connection timeout"

    def validate(self, data) -> bool:
        if type(data) is str:
            return True
        else:
            return False

    def format_output(self, result) -> str:
        out = super().format_output(result)
        return out + "\nTextProcessor done successfully"


class LogProcessor(DataProcessor):
    def process(self, data) -> str:
        return data

    def validate(self, data) -> bool:
        log_headers = ['[ERROR]', '[STATUS]', '[INFO]']
        if type(data) is str:
            for log_h in log_headers:
                if log_h in data:
                    return True
            return False

    def format_output(self, result) -> str:
        return f"Error was catched => {result}"


def polymorphic_way(btch_d: list[any], btch_obj: list[DataProcessor]) -> None:
    for i in range(0, len(btch_d)):
        print(f'Result {i+1}:', end=' ')
        for obj in btch_obj:
            if obj.validate(btch_d[i]):
                print(obj.format_output(obj.process(btch_d[i])))
                break


if __name__ == "__main__":
    lst = [1, 2, 3, 4, 5]
    print("Initializing Numeric Processor...")
    print("Processing data:", lst)
    num = NumericProcessor()
    if num.validate([1, 2, 3, 4, 5]):
        print("Validation: Numeric data verified")
        result = num.format_output(num.process(lst))
        print(result)
    else:
        print("Error: Numeric data unverified")

    text = "Hello Nexus World"
    print("\nInitializing Text Processor...")
    print("Processing data:", text)
    tproc = TextProcessor()
    if tproc.validate(text):
        print("Validation: Text data verified")
        result = tproc.format_output(tproc.process(text))
        print(result)
    else:
        print("Error: Text data unverified")

    log = '[ERROR] invalid data'
    print("\nInitializing Log Processor...")
    print("Processing data:", log)
    logproc = LogProcessor()
    if logproc.validate(log):
        print("Validation: Log data verified")
        result = logproc.format_output(logproc.process(log))
        print(result)
    else:
        print("Error: Log data unverified")

    print('\n=== Polymorphic Processing Demo ===')
    polymorphic_way([lst, log, text], [logproc, tproc, num])
