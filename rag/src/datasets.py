from pydantic import BaseModel, Field, model_validator
import uuid


class MinimalSource(BaseModel):
    file_path: str
    first_character_index: int
    last_character_index: int = Field(default=0)
    text: str

    @model_validator(mode="after")
    def filling_last_character_index(self):
        self.last_character_index = self.first_character_index + len(self.text)
        return self


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
    search_results: list[MinimalSearchResults]
    k: int


class StudentSearchResultsAndAnswer(BaseModel):
    search_results: list[MinimalAnswer]
    k: int
