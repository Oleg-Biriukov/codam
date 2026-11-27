from src.chunker import Indexer, CHUNK_INDEX_FD
from src.datasets import (MinimalSource, RagDataset, StudentSearchResults,
                          StudentSearchResultsAndAnswer)
from src.retriever import Retriever
from src.generator import Generator
from pydantic import BaseModel
from src.dataset_maintainer import Database as Dt

import os

DEFAULT_SAVE_DIRECTORY = "data/output/"


class CLI(BaseModel):
    input_file: str

    def index(self, max_chunk_size: int) -> None:
        chunker: Indexer

        chunker = Indexer.build(self.input_file, max_chunk_size)
        chunker.chunk()
        chunker.process()

    def search(self, query: str, k: int) -> None:
        retriever: Retriever
        sources: list[MinimalSource]

        retriever = Retriever.create(bm25_save_fld=CHUNK_INDEX_FD)
        sources = retriever.score_question(query, k)
        for s in sources:
            print(f"{s.file_path} [{s.first_character_index}:"
                  f"{s.last_character_index}]")

    def search_dataset(self, dataset_path: str, k: int,
                       save_directory: str = DEFAULT_SAVE_DIRECTORY) -> None:
        retriever: Retriever
        sources: StudentSearchResults
        rag_dataset: RagDataset

        retriever = Retriever.create(bm25_save_fld=CHUNK_INDEX_FD)
        rag_dataset = Dt.load_dataset(dataset_path, RagDataset)
        if save_directory == DEFAULT_SAVE_DIRECTORY:
            save_directory = os.path.join(save_directory,
                                          "search_result",
                                          type(rag_dataset.rag_questions
                                               ).__name__)
        sources = retriever.score_datasets(rag_dataset, k)
        Dt.save_datasets(dataset_path, save_directory, sources)

    def answer(self, query: str, k: int) -> None:
        gen: Generator
        retriever: Retriever
        sources: list[MinimalSource]
        prompt: str

        gen = Generator.create()
        retriever = Retriever.create(bm25_save_fld=CHUNK_INDEX_FD)
        sources = retriever.score_question(query, k)
        prompt = gen.format_prompt(query, sources)
        print(gen.answer(prompt))

    def answer_dataset(self, student_search_results_path: str,
                       save_directory: str = DEFAULT_SAVE_DIRECTORY) -> None:
        gen: Generator
        s_res: StudentSearchResults
        answers: StudentSearchResultsAndAnswer

        gen = Generator.create()
        s_res = Dt.load_dataset(student_search_results_path,
                                StudentSearchResults)
        if save_directory == DEFAULT_SAVE_DIRECTORY:
            save_directory = os.path.join(save_directory,
                                          "search_results_and_answer")
        answers = gen.answer_dataset(s_res)
        Dt.save_datasets(student_search_results_path, save_directory, answers)

    def evaluate(self, student_search_results_path: str,
                 dataset_path: str) -> None:
        pass
