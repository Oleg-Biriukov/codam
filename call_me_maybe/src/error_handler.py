from functools import wraps
from pydantic import ValidationError
from json import JSONDecodeError
from typing import Any, Callable


def unpacking_validation_error(e: Any) -> str:
    error_string: str = ""
    loc: str
    msg: str

    for error in e.errors():
        loc = error["loc"][0] + ": " if len(error["loc"]) else ""
        msg = error["msg"]
        error_string += f"({loc}{msg})"
    return error_string


errors: dict = {
    ValueError: lambda txt: txt,
    FileNotFoundError: lambda txt: txt,
    ValidationError: unpacking_validation_error,
    JSONDecodeError: lambda txt: txt
}


def error_handler(func: Callable) -> Callable:
    @wraps(func)
    def wrapper(*args: Any, **kwargs: Any) -> Any:
        try:
            return func(*args, **kwargs)
        except Exception as e:
            if errors.get(type(e)):
                print(f"Error caused: {errors[type(e)](e)}")
            else:
                print(f"New type detected: {type(e).__name__}: {e}")
            exit()
    return wrapper
