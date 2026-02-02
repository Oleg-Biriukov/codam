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
    def __init__(self, stream_id: str):
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
    def __init__(self, stream_id: str):
        self.id = stream_id
        self.lst_data = []
        print(f"Stream ID: {stream_id}, Type: Financial Data")

    def process_batch(self, data_batch: List[Any]) -> str:
        try:
            buy = sum([i for i in data_batch if i > 0])
            sells = sum([i * -1 for i in data_batch if i < 0])
            self.lst_data.append(data_batch)
            return f'''Buy: {buy}\
 | Sells: {sells}\
 | Net flow: {sum(data_batch)}'''
        except Exception:
            return None

    def get_stats(self) -> Dict[str, Union[str, int, float]]:
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
    def __init__(self, stream_id: str):
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

    def get_stats(self) -> Dict[str, Union[str, int, float]]:
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


class StreamProcessor():
    @staticmethod
    def polym_process(btch_obj: list[DataStream]) -> None:
        for obj in btch_obj:
            if isinstance(obj, SensorStream):
                print(f'- Sensor data: {obj.get_stats()['Data processed']}\
 readings processed')

            elif isinstance(obj, TransactionStream):
                print(f'- Transaction data: \
{obj.get_stats()['Data processed']} operations processed')

            elif isinstance(obj, EventStream):
                print(f'- Event data: \
{obj.get_stats()['Data processed']} Event processed')

    @staticmethod
    def filter_data(data_batch: List[DataStream],
                    criteria: Optional[str] = None) -> Dict:
        rtnr = dict(temputare=[],
                    errors=[],
                    transiction=[])

        for obj in data_batch:
            if criteria == 'high-priority':
                if isinstance(obj, SensorStream):
                    for data in obj.lst_data:
                        for tmpr in data:
                            if tmpr > 30:
                                rtnr['temputare'].append(tmpr)
                elif isinstance(obj, TransactionStream):
                    for data in obj.lst_data:
                        for transct in data:
                            if transct > 1000000:
                                rtnr['transiction'].append(transct)
                elif isinstance(obj, EventStream):
                    for data in obj.lst_data:
                        for event in data:
                            if 'fatal' in event or 'critical' in event:
                                rtnr['errors'].append(event)

            elif criteria == 'low-priority':
                if isinstance(obj, SensorStream):
                    for data in obj.lst_data:
                        for tmpr in data:
                            if tmpr < 30:
                                rtnr['temputare'].append(tmpr)
                elif isinstance(obj, TransactionStream):
                    for data in obj.lst_data:
                        for transct in data:
                            if transct < 1000000:
                                rtnr['transiction'].append(transct)
                elif isinstance(obj, EventStream):
                    for data in obj.lst_data:
                        for event in data:
                            if 'fatal' in event or 'critical' in event:
                                continue
                            rtnr['errors'].append(event)

        return rtnr


if __name__ == "__main__":
    obj = []
    data = [[220, 0, 3, 1, -1],
            [100, -100, 3000, -32],
            ['login', 'error', 'logout', 'alert', 'fatal_error']]
    criteria = 'high-priority'

    print('=== CODE NEXUS - POLYMORPHIC STREAM SYSTEM ===')

    print('\nInitializing Sensor Stream...')
    obj.append(SensorStream('SENSOR_001'))
    print('Processing sensor batch:', obj[0].process_batch(data[0]))
    print('Sensor analysis:', obj[0].get_stats())

    print('\nInitializing Transaction Stream...')
    obj.append(TransactionStream('TRANS_001'))
    print('Processing sensor batch:', obj[1].process_batch(data[1]))
    print('Sensor analysis:', obj[1].get_stats())

    print('\nInitializing Event Stream...')
    obj.append(EventStream('EVENT_001'))
    print('Processing sensor batch:', obj[2].process_batch(data[2]))
    print('Sensor analysis:', obj[2].get_stats())

    print('''\n=== Polymorphic Stream Processing ===
Processing mixed stream types through unified interface...\n''')

    print('Batch 1 Results:')
    StreamProcessor.polym_process(obj)

    print("\nStream filtering active:", criteria)
    res = StreamProcessor.filter_data(obj, criteria)
    print(f'''Filtered results:
{len(res['errors'])} critical sensor alerts
{len(res['transiction'])} large transaction
{len(res['temputare'])} anomaly temperature
''')

    print('All streams processed successfully. Nexus throughput optimal')
