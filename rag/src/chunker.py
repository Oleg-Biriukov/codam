from pydantic import BaseModel, Field, model_validator, model_serializer
from typing import ClassVar
from glob import glob
from langchain_text_splitters import Language, RecursiveCharacterTextSplitter
from tqdm import tqdm
import os
import bm25s

# from src.index import BM25Index

CHUNK_INDEX_FD = os.path.abspath("data/processed/index_chunks")


class Chunk(BaseModel):
    file_path: str
    first_character_index: int
    last_character_index: int = 0
    text: str

    @model_validator(mode="after")
    def filling_last_character_index(self):
        self.last_character_index = self.first_character_index + len(self.text)
        return self


class Chunker(BaseModel):
    py_files: list
    md_files: list
    max_size_chunk: int

    chunks: ClassVar[list] = []

    def chunk(self):
        py_splitter: any = RecursiveCharacterTextSplitter.from_language(
                    language=Language.PYTHON,
                    chunk_size=self.max_size_chunk,
                    chunk_overlap=150,
                    add_start_index=True
                )
        md_splitter: any = RecursiveCharacterTextSplitter(
                    chunk_size=self.max_size_chunk,
                    chunk_overlap=150,
                    separators=["\n\n", "\n", " ", ""],
                    add_start_index=True,
                )

        for f in tqdm(self.py_files + self.md_files):
            if '.md' in f:
                Chunker.md_chunker(md_splitter, f)
            else:
                Chunker.py_chunker(py_splitter, f)

    @staticmethod
    def py_chunker(splitter: any, py_file: str):
        content: str

        with open(py_file, "r") as c:
            content = c.read()

        for chunk in splitter.create_documents([content]):
            Chunker.chunks.append(
                Chunk(file_path=py_file,
                      first_character_index=chunk.metadata["start_index"],
                      text=chunk.page_content)
            )

    @staticmethod
    def md_chunker(splitter: any, md_file: str):
        content: str

        with open(md_file, "r") as c:
            content = c.read()

        for chunk in splitter.create_documents([content]):
            Chunker.chunks.append(
                Chunk(file_path=md_file,
                      first_character_index=chunk.metadata["start_index"],
                      text=chunk.page_content)
            )


class Indexer(Chunker):
    @classmethod
    def build(cls, input_file: str, max_size_chunk: int) -> "Indexer":
        abs_path: str = os.path.abspath(input_file)

        if not os.path.exists(abs_path):
            raise FileNotFoundError("Doesnt exist.")

        if max_size_chunk > 2000:
            raise ValueError("size out of limit")

        py_files = sorted(glob(os.path.join(abs_path, "**", "*.py"),
                               recursive=True),
                          key=lambda f: f)
        md_files = sorted(glob(os.path.join(abs_path, "**", "*.md"),
                               recursive=True),
                          key=lambda f: f)

        return cls(py_files=py_files,
                   md_files=md_files,
                   max_size_chunk=max_size_chunk)

    def process(self):
        tokens: any
        retriever: any
        metadata: list = [c.model_dump() for c in Chunker.chunks]

        tokens = bm25s.tokenize([chunk.text for chunk in Chunker.chunks])
        retriever = bm25s.BM25(corpus=metadata)

        retriever.index(tokens, corpus=metadata)
        retriever.save(CHUNK_INDEX_FD)
