from src.chunker import Indexer, CHUNK_INDEX_FD
from src.datasets import MinimalSource, RagDataset, StudentSearchResults, StudentSearchResultsAndAnswer
from src.retriever import Retriever, Generator
from pydantic import BaseModel


class CLI(BaseModel):
    input_file: str

    def index(self, max_chunk_size: int):
        chunker: Indexer

        chunker = Indexer.build(self.input_file, max_chunk_size)
        chunker.chunk()
        chunker.process()

    def search(self, query: str, k: int):
        retriever: Retriever
        sources: list[MinimalSource]

        retriever = Retriever.create(bm25_save_fld=CHUNK_INDEX_FD)
        sources = retriever.score_question(query, k)
        for s in sources:
            print(f"{s.file_path} [{s.first_character_index}:"
                  f"{s.last_character_index}]")

    def search_dataset(self, dataset_path: str, k: int, save_directory: str):
        retriever: Retriever
        sources: StudentSearchResults
        rag_dataset: RagDataset

        retriever = Retriever.create(bm25_save_fld=CHUNK_INDEX_FD)
        rag_dataset = retriever.load_dataset(dataset_path)
        sources = retriever.score_datasets(rag_dataset, k)
        retriever.save_datasets(dataset_path, save_directory, sources)


    def answer(self, query: str, k: int):
        gen: Generator
        retriever: Retriever
        sources: list[MinimalSource]
        prompt: str

        gen = Generator.create()
        retriever = Retriever.create(bm25_save_fld=CHUNK_INDEX_FD)
        sources = retriever.score_question(query, k)
        prompt = gen.format_prompt(query, sources)
        print(gen.answer(prompt))

    def answer_dataset(self, student_search_results_path: str, save_directory: str):
        gen: Generator
        s_res: StudentSearchResults
        answers: StudentSearchResultsAndAnswer

        gen = Generator.create()
        s_res = gen.load_student_search(student_search_results_path)
        answers = gen.answer_dataset(s_res)
        gen.dump_dataset(student_search_results_path, save_directory, answers)

    def evaluate(self, student_search_results_path: str, dataset_path: str):
        pass
