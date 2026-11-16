from src.chunker import Indexer, CHUNK_INDEX_FD
from src.datasets import MinimalSource
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

        retriever = Retriever.create(bm25_save_fld=f"{CHUNK_INDEX_FD}/bm25",
                                     faiss_save_fld=f"{CHUNK_INDEX_FD}/faiss_.index")
        sources = retriever.rrf(query, k)
        # for s in sources:
        #     print(f"{s.file_path} [{s.first_character_index}:"
        #           f"{s.last_character_index}]")

    def search_dataset(self, dataset_path: str, k: int, save_directory: str):
        pass

    def answer(self, query: str, k: int):
        gen: Generator
        retriever: Retriever
        sources: list[MinimalSource]
        prompt: str

        gen = Generator.create()
        retriever = Retriever.create(index_save_fld=CHUNK_INDEX_FD)
        sources = retriever.score_question(query, k)
        prompt = gen.format_prompt(query, sources)
        print(gen.generate(prompt))

    def answer_dataset(self, student_search_results_path: str, save_directory: str):
        pass

    def evaluate(self, student_search_results_path: str, dataset_path: str):
        pass
