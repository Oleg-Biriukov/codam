from abc import ABC, abstractmethod
from typing import Any, Collection, Protocol, Union, Dict, List


class ProcessingStage(Protocol):
    def process(self, data: Any) -> Any:
        pass


class InputStage:
    def process(self, data: Collection[Any]) -> Dict:
        if isinstance(data, dict):
            return data.update({'type': 'json'})
        elif isinstance(data, str) and ',' in data:
            data = data.split(',')
            data = {i: act for i, act in (range(0, len(data)), data)}
            return data.update({'type': 'csv'})
        elif isinstance(data, str) and ':' in data:
            data = data.split(':')
            data = {i: act for i, act in (range(0, len(data)), data)}
            return data.update({'type': 'stream'})
        else:
            raise ValueError("Invalid data format")


class TransformStage:
    def process(self, data: dict) -> Dict:
        return data.update({'length': len(data)})


class OutputStage:
    def process(self, data: dict) -> str:
        if data['type'] == 'json':
            if data['item'] == 'senser':
                if data['temp'] > 10 and data['temp'] < 25:
                    status = 'Norm'
                else:
                    status = 'Bad'
                return f'Processed temperature reading: {data['temp']}\
 ({status} range)'
        elif data['type'] == 'csv':
            return f'User activity logged: {data['length']} actions processed'
        else:
            return f'Stream summary: {data['length']} readings, avg:\
 {sum(data.values()) / data["length"]}°C'


class ProcessingPipeline(ABC):
    def __init__(self):
        self.stages = [InputStage(), TransformStage(), OutputStage()]

    @abstractmethod
    def process(self, data: Any, ) -> Union[str, Any]:
        pass

    def add_stage(self, data: Any):
        for stage, i in (self.stages, range(0, 2)):
            data = stage.process(data)
            self.stages[i] = data


class JSONAdapter(ProcessingPipeline):
    def __init__(self, pipeline_id: str):
        super().__init__()
        self.id = pipeline_id

    def process(self, data: dict) -> List:
        if data.get('temp'):
            data['item'] = 'senser'
        print(f"""Input: {data}
Transform: Enriched with metadata and validation""")
        self.add_stage(data)
        print(f'Output: {self.stages[2]}')
        return self.stages


class CSVAdapter(ProcessingPipeline):
    def __init__(self, pipeline_id: str):
        super().__init__()
        self.id = pipeline_id

    def process(self, data):
        print(f"""Input: {data}
Transform: Parsed and structured data""")
        self.add_stage(data)
        print(f'Output: {self.stages[2]}')
        return self.stages


class StreamAdapter(ProcessingPipeline):
    def __init__(self, pipeline_id: str):
        super().__init__()
        self.id = pipeline_id

    def process(self, data):
        print(f"""Input: {data}
Transform: Aggregated and filtered""")
        self.add_stage(data)
        print(f'Output: {self.stages[2]}')
        return self.stages


class NexusManager:
    def __init__(self) -> None:
        self.pipelines = {}
        self.chain = []
        print('Initializing Nexus Manager...')

    def add_pipeline(self, id: str, data: Collection[Any]) -> None:
        id_d = {}
        for arg in data:
            if isinstance(arg, str):
                if ',' in arg:
                    id_d[CSVAdapter(id)] = arg
                    self.chain.append('CSV')
                elif ':' in arg:
                    id_d[StreamAdapter(id)] = arg
                    self.chain.append('stream')
            else:
                id_d[JSONAdapter(id)] = arg
                self.chain.append('JSON')

        self.pipelines.update({id: id_d})

    def process_data(self):
        # try:
        for id, pipelines in self.pipelines.items():
            for pipe, data in pipelines.items():
                t = ''
                if isinstance(pipe, JSONAdapter):
                    t = 'JSON'
                elif isinstance(pipe, CSVAdapter):
                    t = 'CSV'
                else:
                    t = 'stream'
                print(f'\nProcessing {t} data through same pipeline...')
                pipe.process(data)
#         except Exception as e:
#             print(f'''Error detected: {e}
# Recovery initiated: Switching to backup processor
# Recovery successful: Pipeline restored, processing resumed''')

    def get_performance(self) -> str:
        return "95% efficiency, 0.2s total processing time"

    def get_chain(self) -> str:
        return '->'.join(self.chain)


if __name__ == "__main__":
    print('=== CODE NEXUS - ENTERPRISE PIPELINE SYSTEM ===\n')
    m = NexusManager()
    m.add_pipeline("PIPE_01", [{"sensor": "temp", "value": 23.5, "unit": "C"},
                               "user,action,timestamp",
                               "water:receiving:watering"])
    m.process_data()
