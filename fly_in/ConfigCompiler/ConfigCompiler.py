from pydantic import BaseModel
from typing import TypedDict, List, ClassVar
from hubs.hub import Hub
from drons.dron import Dron
from ConfigCompiler.ErrorPrompts import HubError, ConfError


class DataConf(TypedDict):
    drons: List[Dron]
    start_hub: Hub
    end_hub: Hub
    hubs: List[Hub | None]


class Meta(TypedDict):
    zone: str | None
    color: str | None
    max_drones: int | None
    max_link_capacity: int | None


class ConMeta(TypedDict):
    max_link_capacity: int | None


class ConfigCompliler(BaseModel):
    path: ClassVar[str] = ''

    @classmethod
    def modify_path(cls, new_name: str) -> None:
        cls.path = new_name

    @classmethod
    def get_values(cls) -> DataConf | dict:
        def search_hub(name: str, hubs: List[Hub]) -> Hub | None:
            for hub in hubs:
                if hub.name == name:
                    return hub
            return None

        data: DataConf = {'hubs': []}
        arg: List[str]
        br: str
        try:
            with open(cls.path, 'r') as conf:
                for line in conf:
                    line = line.strip()
                    if line[0] != '#':
                        name_arg: List[str] = line.split(': ')
                        if name_arg[0] == 'start_hub':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = dict(var.split('=') for var in br.split())

                            data['start_hub'] = Hub(name=arg[0],
                                                    pos=(int(arg[1]),
                                                         int(arg[2])),
                                                    **meta)
                        elif name_arg[0] == 'end_hub':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = dict(var.split('=') for var in br.split())

                            data['end_hub'] = Hub(name=arg[0],
                                                  pos=(int(arg[1]),
                                                       int(arg[2])),
                                                  **meta)
                        elif name_arg[0] == 'hub':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = dict(var.split('=') for var in br.split())

                            data['hubs'].append(Hub(name=arg[0],
                                                    pos=(int(arg[1]),
                                                         int(arg[2])),
                                                    **meta))
                        elif name_arg[0] == 'connection':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[1::]).strip('[]')

                            meta = dict(var.split('=') for var in br.split())

                            con: List[str] = arg[0].split('-')
                            hub_con: List[Hub] = list(map(search_hub, con))
                            if None not in hub_con:
                                hub_con[0].next = hub_con[1]
                                hub_con[1].prev = hub_con[0]
                            else:
                                raise HubError('wrong name for connection')

                        else:
                            raise ConfError(f'not appropriate type of \
variable({name_arg[0]})')

        except Exception as e:
            print(f'{type(e).__name__}: {e}')

        return data
