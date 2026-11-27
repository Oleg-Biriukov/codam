from typing import Any, Callable
from pydantic import BaseModel
import json
import os


class Database(BaseModel):
    @staticmethod
    def load_dataset(dataset_dir: str, dataset: Callable) -> Any:

        with open(dataset_dir, "r") as f:
            return dataset(**json.load(f))

    @staticmethod
    def save_datasets(dataset_dir: str, save_dir: str,
                      dataset: Any) -> None:
        _, filename = os.path.split(dataset_dir)

        with open(os.path.join(save_dir, filename), "w+") as f:
            json.dump(dataset.model_dump(), f, indent=4)
