from pydantic import BaseModel, Field, model_validator, model_serializer
from typing import ClassVar
import uuid
import os


class MinimalSource(BaseModel):
    counter: ClassVar[int] = 0
    id: int = Field(default=0)
    file_path: str
    first_character_index: int
    last_character_index: int = Field(default=0)
    text: str = Field(default="")

    @model_validator(mode="after")
    def filling_last_character_index(self) -> "MinimalSource":
        self.id = MinimalSource.counter
        self.last_character_index = self.first_character_index + len(self.text)
        MinimalSource.counter += 1
        if self.text != "":
            return self
        with open(self.file_path, "r") as f:
            self.text = f.read()[self.first_character_index:
                                 self.last_character_index]
        return self

    @model_serializer(mode="plain")
    def serializer(self):
        return {
            "file_path": os.path.relpath(self.file_path, os.getcwd()),
            "first_character_index": self.first_character_index,
            "last_character_index": self.last_character_index,
        }


class UnansweredQuestion(BaseModel):
    question_id: str = Field(default_factory=lambda: str(uuid.uuid4()))
    question: str


class AnsweredQuestion(UnansweredQuestion):
    sources: list[MinimalSource]
    answer: str


class RagDataset(BaseModel):
    rag_questions: list[AnsweredQuestion | UnansweredQuestion]


class MinimalSearchResults(BaseModel):
    question_id: str
    question: str
    retrieved_sources: list[MinimalSource]


class MinimalAnswer(MinimalSearchResults):
    answer: str


class StudentSearchResults(BaseModel):
    search_results: list[MinimalSearchResults] = Field(
        default_factory=list[MinimalSearchResults])
    k: int


class StudentSearchResultsAndAnswer(BaseModel):
    search_results: list[MinimalAnswer] = Field(
        default_factory=list[MinimalAnswer])
    k: int
