from abc import ABC, abstractmethod
from typing import Any, Optional, List, Union, Dict


class DataStream(ABC):
    @abstractmethod
    def process_batch(self, data_batch: List[Any]) -> str:
        pass

    def filter_data(self, data_batch: List[Any], criteria: Optional[str]
                    = None) -> List[Any]:
        self.data = data_batch
        if criteria:
            if criteria == "rev":
                return data_batch.sort(reverse=True)
            return data_batch
        else:
            return data_batch.sort()

    def get_stats(self) -> Dict[str, Union[str, int, float]]:
        return {"Operations": len(self.data)}


class SensorStream(DataStream):
    def __init__(self, stream_id):
        self.id = stream_id

    def process_batch(self, data_batch: List[Any]) -> str:
        try:
            avg_tmp = sum(data_batch) / len(data_batch)
            return f"{len(data_batch)} readings processed,\
 avg temp: {avg_tmp}°C"
        except Exception as e:
            return f"Error: {e}"

    def get_stats(self):
        try:
            return {
                "Count regions": len(self.data)
            }
        except Exception:
            return {None}


class TransactionStream(DataStream):
    pass


class EventStream(DataStream):
    pass


class StreamProcessor:
    pass
