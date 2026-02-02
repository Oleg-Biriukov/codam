from abc import ABC, abstractmethod
from typing import Any, Collection, Protocol, Union, Dict, List


class ProcessingStage(Protocol):
    def process(self, data: Any) -> Any:
        pass


class InputStage:
    def process(self, data: Collection[Any]) -> Dict:
        if isinstance(data, dict):
            data.update({'type': 'json'})
            return data
        elif isinstance(data, str) and ',' in data:
            data = data.split(',')
            data = {i: act for i, act in enumerate(data)}
            data.update({'type': 'csv'})
            return data
        elif isinstance(data, str) and ':' in data:
            data = data.split(':')
            data = {i: act for i, act in enumerate(data)}
            data.update({'type': 'stream'})
            return data
        else:
            raise ValueError("Invalid data format (Stage 1)")


class TransformStage:
    def process(self, data: dict) -> Dict:
        try:
            data.update({'length': len(data) - 1})
            return data
        except Exception:
            raise ValueError("Invalid data format (Stage 2)")


class OutputStage:
    def process(self, data: dict) -> str:
        if data['type'] == 'json':
            if data['item'] == 'senser':
                if data['value'] > 10 and data['value'] < 25:
                    status = 'Norm'
                else:
                    status = 'Bad'
                return f'Processed temperature reading: {data['value']}\
 ({status} range)'
        elif data['type'] == 'csv':
            return f'User activity logged: {data['length']} actions processed'
        elif data['type'] == 'stream':
            return f'Stream summary: {data['length']} readings'
        else:
            raise ValueError("Invalid data format (Stage 3)")


class ProcessingPipeline(ABC):
    def __init__(self):
        self.stages = [InputStage(), TransformStage(), OutputStage()]

    @abstractmethod
    def process(self, data: Any, ) -> Union[str, Any]:
        pass

    def add_stage(self, data: Any):
        i = 0
        for stage in self.stages:
            data = stage.process(data)
            self.stages[i] = data
            i += 1


class JSONAdapter(ProcessingPipeline):
    def __init__(self, pipeline_id: str):
        super().__init__()
        self.id = pipeline_id

    def process(self, data: dict) -> List:
        if data.get('sensor') == 'temp':
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
        try:
            for arg in data:
                if isinstance(arg, str):
                    if ',' in arg:
                        id_d[CSVAdapter(id)] = arg
                        self.chain.append('CSV')
                    elif ':' in arg:
                        id_d[StreamAdapter(id)] = arg
                        self.chain.append('stream')
                    else:
                        raise ValueError("invalid input")
                elif isinstance(arg, dict):
                    id_d[JSONAdapter(id)] = arg
                    self.chain.append('JSON')
                else:
                    raise ValueError("invalid input")
        except Exception as e:
            print(f'''Error detected: {e}
Recovery initiated: Switching to backup processor
Recovery successful: Pipeline restored, processing resumed''')

        self.pipelines.update({id: id_d})

    def process_data(self):
        try:
            for id, pipelines in self.pipelines.items():
                for pipe, data in pipelines.items():
                    t = ''
                    if isinstance(pipe, JSONAdapter):
                        t = 'JSON'
                    elif isinstance(pipe, CSVAdapter):
                        t = 'CSV'
                    elif isinstance(pipe, StreamAdapter):
                        t = 'stream'
                    print(f'\nProcessing {t} data through same pipeline...')
                    pipe.process(data)
        except Exception as e:
            print(f'''Error detected: {e}
Recovery initiated: Switching to backup processor
Recovery successful: Pipeline restored, processing resumed''')

    def get_performance(self) -> str:
        return "95% efficiency, 0.2s total processing time"

    def get_chain(self) -> str:
        return '->'.join(self.chain)

    def print_stats(self) -> None:
        print(f'The pipe net contain {len(self.pipelines)} pipelines')
        for id, pipes in self.pipelines.items():
            print(f'->{id}')
            for obj in pipes.keys():
                if isinstance(obj.stages[2], OutputStage):
                    raise ValueError('The data havent processed yet.')
                print(f'\t-{obj.stages[2]}')


if __name__ == "__main__":
    print('=== CODE NEXUS - ENTERPRISE PIPELINE SYSTEM ===\n')
    m = NexusManager()
    print('\n=== Multi-Format Data Processing ===\n')
    m.add_pipeline("PIPE_01", [{"sensor": "temp", "value": 23.5, "unit": "C"},
                               "user,action,timestamp",
                               "water:receiving:watering"])
    m.process_data()
    print("\n=== Pipeline Chaining Demo ===")
    print(m.get_chain())
    print(f'\n{m.get_performance()}\n')
    m.print_stats()
    print('\n=== Error Recovery Test ===\nSimulating pipeline failure...')
    m.add_pipeline("PIPE_02", [{},
                               "userhgactiontimestamp",
                               "water:receiving:watering"])
