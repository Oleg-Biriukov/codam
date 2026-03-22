from pydantic import BaseModel
from typing import TypedDict, List, ClassVar, cast
from hubs.hub import Hub, Zone, Color
from drons.dron import Dron
from .ErrorPrompts import HubError, ConfError
from fly_in import DataConf


class Meta(TypedDict):
    zone: Zone
    color: Color
    max_drones: str
    max_link_capacity: str


class ConMeta(TypedDict):
    max_link_capacity: int | None


class ConfigCompiler(BaseModel):
    path: ClassVar[str] = ''

    @classmethod
    def modify_path(cls, new_name: str) -> None:
        cls.path = new_name

    @classmethod
    def get_values(cls) -> DataConf:
        def search_hub(name: str, hubs: List[Hub]) -> Hub | None:
            for hub in hubs:
                if hub.name == name:
                    return hub
            return None

        data: DataConf = {'hubs': [],
                          'dron': [],
                          'start_hub': None,
                          'end_hub': None}
        arg: List[str]
        br: str
        meta: Meta
        try:
            with open(cls.path, 'r') as conf:
                for line in conf:
                    line = line.strip()
                    if line[0] != '#':
                        name_arg: List[str] = line.split(': ')

                        if (name_arg[0] == 'start_hub' and
                                           data['start_hub'] is None):

                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = cast(Meta, dict(var.split('=')
                                        for var in br.split()))

                            data['start_hub'] = Hub(name=arg[0],
                                                    pos=(int(arg[1]),
                                                         int(arg[2])),
                                                    **meta)

                            data['hubs'].append(data['start_hub'])

                        elif (name_arg[0] == 'end_hub' and
                                             data['end_hub'] is None):

                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = cast(Meta, dict(var.split('=')
                                        for var in br.split()))

                            data['end_hub'] = Hub(name=arg[0],
                                                  pos=(int(arg[1]),
                                                       int(arg[2])),
                                                  **meta)

                            data['hubs'].append(data['end_hub'])

                        elif name_arg[0] == 'hub':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = cast(Meta, dict(var.split('=')
                                        for var in br.split()))

                            data['hubs'].append(Hub(name=arg[0],
                                                    pos=(int(arg[1]),
                                                         int(arg[2])),
                                                    **meta))
                        elif name_arg[0] == 'connection':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[1::]).strip('[]')

                            con: List[str] = arg[0].split('-')
                            hub_con: List[Hub | None] = [search_hub(
                                c,
                                cast(List[Hub],
                                     data['hubs']))
                                                         for c in con]
                            if None not in hub_con:
                                connection: List[Hub] = cast(List[Hub],
                                                             hub_con)
                                if br:
                                    mx_c: str = br.split('=')[1]
                                    connection[0].max_link_capacity = mx_c
                                connection[0].next.append(connection[1])
                                connection[1].prev.append(connection[0])
                            else:
                                raise HubError('wrong name for connection')
                        elif name_arg[0] == '':
                            continue
                        elif name_arg[0] == 'nb_drones' and data['dron'] == []:
                            for d in range(int(name_arg[1])):
                                data['dron'].append(Dron(id=d))
                        else:
                            raise ConfError(f'not appropriate type of \
variable({name_arg[0]})')

        except Exception as e:
            print(f'{type(e).__name__}: {e}')

        return data
