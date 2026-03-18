class HubError(Exception):
    def __init__(self, message):
        super().__init__(message)


class ConfError(SyntaxError):
    def __init__(self, message):
        super().__init__(message)
