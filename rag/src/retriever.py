from typing import Any
from pydantic import BaseModel
import bm25s
from src.datasets import MinimalSource
import os


SAVE_MINIMAL_SEARCH = os.path.abspath("data/Un")


class Retriever(BaseModel):
    retriever: Any

    @classmethod
    def create(cls, index_save_fld: str) -> "Retriever":
        retriever: Any

        if not os.path.exists(index_save_fld):
            raise FileExistsError("Do index first.")
        retriever = bm25s.BM25().load(index_save_fld, load_corpus=True)
        return cls(retriever=retriever)

    def score_question(self, question: str, k: int) -> list[MinimalSource]:
        sources: list[MinimalSource] = list()

        results, _ = self.retriever.retrieve(
            bm25s.tokenize(question),
            k=k
        )
        for chunk in results[0]:
            sources.append(MinimalSource(**chunk))
        return sources

    def save_datasets():
        pass

    def score_datasets():
        pass


class Generator(BaseModel):
    pass
