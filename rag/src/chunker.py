from pydantic import BaseModel
from glob import glob
import os
from langchain_text_splitters import Language, RecursiveCharacterTextSplitter


class Chunker(BaseModel):
    input_file: str
    chunk_list_py: list = []
    chunk_list_md: list = []

    @classmethod
    def create(cls, input_file: str):
        abs_path: str = os.path.abspath(input_file) 
        py_files: list
        md_files: list

        if not os.path.exists(abs_path):
            raise FileNotFoundError("Doesnt exist.")

        py_files = glob(os.path.join(abs_path, "**", "*.py"), recursive=True)
        md_files = glob(os.path.join(abs_path, "**", "*.md"), recursive=True)
        

    @staticmethod
    def py_chunker(self):
        pass

    @staticmethod
    def md_chunker(self):
        pass
