from functools import wraps


errors: dict = {
    ValueError: "test"
}


def error_handler(func: callable) -> callable:
    @wraps(func)
    def wrapper(*args, **kwargs):
        try:
            return func(*args, **kwargs)
        except Exception as e:
            if errors.get(e):
                print(f"Error caused: {errors[e]}")
            else:
                print(f"New type detected: {type(e).__name__}: {e}")
    return wrapper
