from pydantic import BaseModel
from glob import glob
import os
from langchain_text_splitters import Language, RecursiveCharacterTextSplitter
import json
from tqdm import tqdm

CHUNK_FD = os.path.abspath("data/processed/chunks.json")


def find_occ(file_name: str, search_word: str):
    with open(file_name, "r") as f:
        for line in f:
            if search_word in line:
                return line


class Chunker(BaseModel):
    py_files: list
    md_files: list

    @classmethod
    def create(cls, input_file: str) -> any:
        abs_path: str = os.path.abspath(input_file)

        if not os.path.exists(abs_path):
            raise FileNotFoundError("Doesnt exist.")

        py_files = sorted(glob(os.path.join(abs_path, "**", "*.py"),
                               recursive=True),
                          key=lambda f: f)
        md_files = sorted(glob(os.path.join(abs_path, "**", "*.md"),
                               recursive=True),
                          key=lambda f: f)
        return cls(py_files=py_files, md_files=md_files)

    def build(self, max_chunk_size: int):
        py_splitter: any = RecursiveCharacterTextSplitter.from_language(
                    language=Language.PYTHON,
                    chunk_size=max_chunk_size,
                    chunk_overlap=150
                )
        md_splitter: any = RecursiveCharacterTextSplitter(
                    chunk_size=max_chunk_size,
                    chunk_overlap=150,
                    separators=["\n\n", "\n", " ", ""]
                )

        for f in tqdm(self.py_files + self.md_files):
            if not find_occ(CHUNK_FD, f):
                if '.md' in f:
                    Chunker.md_chunker(md_splitter, f)
                else:
                    Chunker.py_chunker(py_splitter, f)

    @staticmethod
    def py_chunker(splitter: any, py_file: str):
        chunk_d: dict = {
            "source": None,
            "mtime": None,
            "chunk": None
        }
        content: str

        with open(CHUNK_FD, "a") as data:
            with open(py_file, "r") as c:
                content = c.read()
            chunk_d["source"] = py_file
            chunk_d["mtime"] = os.path.getmtime(py_file)
            chunk_d["chunk"] = splitter.create_documents(content)
            chunk_d["chunk"] = [c.page_content for c in chunk_d["chunk"]]
            data.write(json.dumps(chunk_d, ensure_ascii=False) + '\n')

    @staticmethod
    def md_chunker(splitter: any, md_file: str):
        chunk_d: dict = {
            "source": None,
            "chunk": None,
            "mtime": None
        }
        content: str
        with open(CHUNK_FD, "a") as data:
            with open(md_file, "r") as c:
                content = c.read()
            chunk_d["source"] = md_file
            chunk_d["mtime"] = os.path.getmtime(md_file)
            chunk_d["chunk"] = splitter.create_documents(content)
            chunk_d["chunk"] = [c.page_content for c in chunk_d["chunk"]]
            data.write(json.dumps(chunk_d, ensure_ascii=False) + '\n')
