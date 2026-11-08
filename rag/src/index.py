from pydantic import BaseModel
import bm25s


class BM25Index(BaseModel, bm25s):
    def search(self, query: str, k: int):
        pass
