from pydantic import BaseModel, model_validator
from typing import ClassVar
from glob import glob
from langchain_text_splitters import Language, RecursiveCharacterTextSplitter
from sentence_transformers import SentenceTransformer as ST
from tqdm import tqdm
import os
import numpy as np
import faiss
import bm25s
import pickle

from src.datasets import MinimalSource

CHUNK_INDEX_FD = os.path.abspath("data/processed/index_chunks") + "/bm25"
PICKLE_FD = os.path.abspath("data/processed/index_chunks") + '/pickle_obj'

os.makedirs(CHUNK_INDEX_FD, exist_ok=True)

os.makedirs(PICKLE_FD, exist_ok=True)


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
        chunk: MinimalSource

        with open(py_file, "r") as c:
            content = c.read()

        for c in splitter.create_documents([content]):
            chunk = MinimalSource(file_path=py_file,
                                  first_character_index=c.metadata["start_index"],
                                  text=c.page_content)
            Chunker.chunks.append(chunk)
            with open(PICKLE_FD + f'/chunk{chunk.id}.pkl', "wb") as file:
                pickle.dump(chunk, file)

    @staticmethod
    def md_chunker(splitter: any, md_file: str):
        content: str
        chunk: MinimalSource

        with open(md_file, "r") as c:
            content = c.read()

        for c in splitter.create_documents([content]):
            chunk = MinimalSource(file_path=md_file,
                                  first_character_index=c.metadata["start_index"],
                                  text=c.page_content)
            Chunker.chunks.append(chunk)
            with open(PICKLE_FD + f'/chunk{chunk.id}.pkl', "wb") as file:
                pickle.dump(chunk, file)


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
        chunks: list = [chunk.text for chunk in Chunker.chunks]

        def build_bm25_indexes() -> None:
            tokens: any
            retriever: any

            metadata: list = [c.model_dump() for c in Chunker.chunks]

            tokens = bm25s.tokenize(chunks)
            retriever = bm25s.BM25(corpus=metadata)

            retriever.index(tokens)
            retriever.save(CHUNK_INDEX_FD)

        print("bm25 indexing ...")
        build_bm25_indexes()
