from abc import ABC, abstractmethod
from typing import Any, Optional, List, Union, Dict


class DataStream(ABC):
    @abstractmethod
    def process_batch(self, data_batch: List[Any]) -> str:
        pass

    def filter_data(self, data_batch: List[Any], criteria: Optional[str]
                    = None) -> List[Any]:
        if criteria == "high-priority":
            data_batch.sort(reverse=True)
            return data_batch
        else:
            data_batch.sort()
            return data_batch

    def get_stats(self) -> Dict[str, Union[str, int, float]]:
        try:
            stats = {"Data processed": len(self.lst_data)}
            return stats
        except Exception:
            return None


class SensorStream(DataStream):
    def __init__(self, stream_id):
        self.id = stream_id
        self.lst_data = []
        print(f"Stream ID: {stream_id}, Type: Environmental Data")

    def process_batch(self, data_batch: List[Any]) -> str:
        try:
            avg_tmp = sum(data_batch) / len(data_batch)
            self.lst_data.append(data_batch)
            return f"{len(data_batch)} readings processed,\
 avg temp: {avg_tmp}°C"
        except Exception:
            return None

    def get_stats(self) -> Dict[str, Union[str, int, float]]:
        stats = super().get_stats()
        try:
            sum_t = 0
            len_r = 0
            for i in self.lst_data:
                sum_t += sum(i)
                len_r += len(i)
            stats.update({"Avarage temperature": sum_t/len(self.lst_data),
                          "Total regions": len_r})
        except Exception:
            pass
        return stats


class TransactionStream(DataStream):
    def __init__(self, stream_id):
        self.id = stream_id
        self.lst_data = []
        print(f"Stream ID: {stream_id}, Type: Financial Data")

    def process_batch(self, data_batch):
        try:
            buy = sum([i for i in data_batch if i > 0])
            sells = sum([i * -1 for i in data_batch if i < 0])
            self.lst_data.append(data_batch)
            return f'''Buy: {buy}
Sells: {sells}
Net flow: {sum(data_batch)}'''
        except Exception:
            return None

    def get_stats(self):
        stats = super().get_stats()
        try:
            buys_t = 0
            sells_t = 0
            net_t = 0
            for data_batch in self.lst_data:
                buys_t += sum([i for i in data_batch if i > 0])
                sells_t += sum([i * -1 for i in data_batch if i < 0])
                net_t += sum(data_batch)
            stats.update({
                'Buys total': buys_t,
                'Sells total': sells_t,
                'Net total': net_t})
        except Exception:
            pass
        return stats


class EventStream(DataStream):
    def __init__(self, stream_id):
        self.lst_data = []
        self.id = stream_id
        print(f"Stream ID: {stream_id}, Type: System Events")

    def process_batch(self, data_batch: List[Any]) -> str:
        try:
            errors = 0
            for event in data_batch:
                if 'error' in event:
                    errors += 1
            self.lst_data.append(data_batch)
            return f'{len(data_batch)} events, {errors} error detected'
        except Exception:
            pass
        return None

    def get_stats(self):
        stats = super().get_stats()
        try:
            t_events = 0
            t_errors = 0
            for data_batch in self.lst_data:
                for event in data_batch:
                    if 'error' in event:
                        t_errors += 1
                    t_events += 1
            stats.update({
                'Events total': t_events,
                'Errors total': t_errors})
        except Exception:
            pass
        return stats


class StreamProcessor(DataStream):
    pass


if __name__ == "__main__":
    pass
