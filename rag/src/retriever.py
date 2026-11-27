from typing import Any
from pydantic import BaseModel
from tqdm import tqdm
import bm25s
import os

from src.datasets import (MinimalSource,
                          RagDataset, StudentSearchResults,
                          MinimalSearchResults)


class HybrydSearch(BaseModel):
    bm25: Any

    def get_bm25_rank(self, question: str, k: int) -> list[MinimalSource]:
        sources: list[MinimalSource] = list()

        results, _ = self.bm25.retrieve(
            bm25s.tokenize(question),
            k=k
        )
        for chunk in results[0]:
            sources.append(MinimalSource(**chunk))
        return sources


class Retriever(HybrydSearch):
    @classmethod
    def create(cls, bm25_save_fld: str) -> "Retriever":
        bm25: Any

        if not os.path.exists(bm25_save_fld):
            raise FileExistsError("Do index first.")
        bm25 = bm25s.BM25().load(bm25_save_fld, load_corpus=True)
        return cls(bm25=bm25)

    def score_question(self, question: str, k: int) -> list[MinimalSource]:
        sources: list[MinimalSource]

        sources = self.get_bm25_rank(question=question, k=k, )
        return sources

    def score_datasets(self, dataset: RagDataset,
                       k: int) -> StudentSearchResults:
        q_res: MinimalSearchResults
        l_res: StudentSearchResults

        l_res = StudentSearchResults(k=k)
        for q in tqdm(dataset.rag_questions, unit="qst",
                      desc="ranking questions"):
            q_res = MinimalSearchResults(
                question_id=q.question_id,
                question=q.question,
                retrieved_sources=self.score_question(q.question, k)
            )
            l_res.search_results.append(q_res)
        return l_res
