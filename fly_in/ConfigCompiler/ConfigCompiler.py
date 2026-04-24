from pydantic import BaseModel
from typing import TypedDict, List, ClassVar, cast
from hubs.hub import Hub, Zone, Color, Dron
from .ErrorPrompts import HubError, ConfError


class DataConf(TypedDict):
    dron: List[Dron]
    start_hub: Hub | None
    end_hub: Hub | None
    hubs: List[Hub | None]


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
        count_drons: int = 0
        try:
            with open(cls.path, 'r') as conf:
                for line in conf:
                    line = line.strip()
                    if line == '':
                        continue
                    if line[0] != '#':
                        name_arg: List[str] = line.split(': ')

                        if (name_arg[0] == 'start_hub' and
                                           data['start_hub'] is None):

                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = cast(Meta, dict(var.split('=')
                                        for var in br.split()))

                            meta['max_drones'] = 'endless'

                            data['start_hub'] = Hub(name=arg[0],
                                                    pos=(int(arg[1]) * 400,
                                                         int(arg[2]) * 400),
                                                    **meta)

                            data['hubs'].append(data['start_hub'])

                        elif (name_arg[0] == 'end_hub' and
                                             data['end_hub'] is None):

                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = cast(Meta, dict(var.split('=')
                                        for var in br.split()))

                            meta['max_drones'] = 'endless'

                            data['end_hub'] = Hub(name=arg[0],
                                                  pos=(int(arg[1]) * 400,
                                                       int(arg[2]) * 400),
                                                  **meta)

                            data['hubs'].append(data['end_hub'])

                        elif name_arg[0] == 'hub':
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[3::]).strip('[]')

                            meta = cast(Meta, dict(var.split('=')
                                        for var in br.split()))

                            data['hubs'].append(Hub(name=arg[0],
                                                    pos=(int(arg[1]) * 400,
                                                         int(arg[2]) * 400),
                                                    **meta))
                        elif name_arg[0] == 'connection':
                            mx_c: str
                            arg = name_arg[1].strip().split(' ')

                            br = ' '.join(arg[1::]).strip('[]')

                            con: List[str] = arg[0].split('-')
                            hub_con: List[Hub | None] = [search_hub(
                                c,
                                cast(List[Hub],
                                     data['hubs']))
                                                         for c in con]
                            if None not in hub_con or len(set(con)) < 2:
                                conct: List[Hub] = cast(List[Hub],
                                                        hub_con)
                                if br:
                                    mx_c = br.split('=')[1]
                                else:
                                    mx_c = '1'
                                if (conct[0].add_next((conct[1], mx_c)) or
                                        conct[1].add_next((conct[0], mx_c))):
                                    raise ConnectionError('Wrong meta-data')
                            else:
                                raise HubError('wrong name for connection')
                        elif name_arg[0] == 'nb_drones' and data['dron'] == []:
                            count_drons = int(name_arg[1])
                        else:
                            raise ConfError(f'not appropriate type of \
variable({name_arg[0]})')

        except Exception as e:
            import traceback
            traceback.print_exc()
            print(f'{type(e).__name__}: {e}')

        for d in range(count_drons):
            x, y = data['start_hub'].pos
            data['dron'].append(Dron(id=d,
                                     c_pos=(x-1, y-1),
                                     pos=data['start_hub']))
        
        # all_connection: set[Hub] = {n for h in data['hubs']
        #                             for n in h.next}

        return data
