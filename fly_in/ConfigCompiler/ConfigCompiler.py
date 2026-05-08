from pydantic import BaseModel, ValidationError
from typing import List, ClassVar
from hubs.hub import Hub, Dron
from DataPrompts import HubError, ConfError, Meta


class ConfigCompiler(BaseModel):
    path: ClassVar[str] = ''
    META_DATA: ClassVar[List] = ['color', 'zone', 'max_drones']

    @classmethod
    def modify_path(cls, new_name: str) -> None:
        cls.path = new_name

    @classmethod
    def get_values(cls) -> dict:
        def search_hub(name: str, hubs: List) -> Hub | None:
            for hub in hubs:
                if hub.name == name:
                    return hub
            return None

        def validate_meta(arg: list) -> dict:
            meta: dict

            if len(arg[:3]) != 3:
                text_error = f'{line_number}: incorrect hub definition'
                raise HubError(text_error)

            br = ' '.join(arg[3::])

            if br != '':
                if (br[0] != '[' or br[::-1][0] != ']' or
                   br.count('[') > 1 or br.count(']') > 1):
                    text_error = f'{line_number}: meta was incorrect defined'
                    raise HubError(text_error)

            br = br.strip('[]')

            try:
                meta = dict(var.split('=')
                            for var in br.split())
            except Exception:
                text_error = f'{line_number}: wrong meta\
 variable'
                raise HubError(text_error)

        # validation meta variables
            for var_m in meta.keys():
                if var_m not in cls.META_DATA:
                    text_error = f'{line_number}: wrong meta\
 variable'
                    raise HubError(text_error)
            return meta

        data: dict = {'hubs': [],
                      'dron': [],
                      'start_hub': None,
                      'end_hub': None}
        arg: list
        meta: dict
        br: str
        count_drons: int = 0
        line_number: int = 0
        con_list: list = []
        text_error: str

        with open(cls.path, 'r') as conf:
            for line in conf:
                line = line.strip()
                line_number += 1
                if line == '':
                    continue

                if line[0] != '#':
                    name_arg: List[str] = line.split(': ')

                    if (name_arg[0] == 'start_hub' and
                                       data['start_hub'] is None
                                       and count_drons > 0):

                        arg = name_arg[1].strip().split(' ')

                        meta = validate_meta(arg)

                        meta['max_drones'] = 'endless'

                        if meta.get('zone') == 'blocked':
                            raise HubError(f'{line_number}: start hub cannot\
 be blocked')

                        if arg[0] in [h.name for h in data['hubs']]:
                            raise HubError(f'{line_number}: same name')

                        try:
                            data['start_hub'] = Hub(name=arg[0],
                                                    pos=(int(arg[1]) * 400,
                                                         int(arg[2]) * 400),
                                                    **meta)
                        except Exception as e:
                            text_error = f'{line_number}: '
                            if isinstance(e, ValidationError):
                                text_error += 'invalid meta data'
                            elif isinstance(e, ValueError):
                                text_error += 'invalid position'
                            raise HubError(text_error)

                        data['hubs'].append(data['start_hub'])

                    elif (name_arg[0] == 'end_hub' and
                          data['end_hub'] is None and
                          count_drons > 0):

                        arg = name_arg[1].strip().split(' ')

                        meta = validate_meta(arg)

                        meta['max_drones'] = 'endless'

                        if arg[0] in [h.name for h in data['hubs']]:
                            raise HubError(f'{line_number}: same name')

                        try:
                            data['end_hub'] = Hub(name=arg[0],
                                                  pos=(int(arg[1]) * 400,
                                                       int(arg[2]) * 400),
                                                  **meta)
                        except Exception as e:
                            text_error = f'{line_number}: '
                            if isinstance(e, ValidationError):
                                text_error += 'invalid meta data'
                            elif isinstance(e, ValueError):
                                text_error += 'invalid position'
                            raise HubError(text_error)

                        data['hubs'].append(data['end_hub'])

                    elif (name_arg[0] == 'hub' and count_drons > 0 and
                          data['start_hub'] is not None):
                        arg = name_arg[1].strip().split(' ')

                        meta = validate_meta(arg)

                        if arg[0] in [h.name for h in data['hubs']]:
                            raise HubError(f'{line_number}: same name')

                        try:
                            data['hubs'].append(Hub(name=arg[0],
                                                    pos=(int(arg[1]) * 400,
                                                         int(arg[2]) * 400),
                                                    **meta))
                        except Exception as e:
                            text_error = f'{line_number}: '
                            if isinstance(e, ValidationError):
                                text_error += 'invalid meta data'
                            elif isinstance(e, ValueError):
                                text_error += 'invalid position'
                            raise HubError(text_error)

                    elif name_arg[0] == 'connection' and count_drons > 0:
                        mx_c: str
                        arg = name_arg[1].strip().split(' ')

                        br = ' '.join(arg[1::])

                        if br != '':
                            if (br[0] != '[' or br[::-1][0] != ']' or
                               br.count('[') > 1 or br.count(']') > 1 or
                               len(arg) != 2):
                                text_error = f'{line_number}: meta was\
 incorrect defined'
                                raise ConnectionError(text_error)

                        br = br.strip('[]')
                        if br != '':
                            if (br.split('=')[0] != 'max_link_capacity' or
                               len(br.split()) != 1):
                                text_error = f'{line_number}: wrong meta\
 variable'
                                raise ConnectionError(text_error)

                        con: List[str] = arg[0].split('-')

                        if sorted(con) in con_list:
                            raise ConnectionError(f'{line_number}\
 same connections')
                        else:
                            con_list.append(sorted(con))

                        if len(set(con)) != 2:
                            raise ConnectionError(f'{line_number}: not\
 appropriate {name_arg[0]} definition')

                        conct: List = [
                            search_hub(c, data['hubs'])
                            for c in con]

                        if None not in conct or len(con) < 2:
                            if br:
                                mx_c = br.split('=')[1]
                            else:
                                mx_c = '1'

                            try:
                                int(mx_c)
                            except Exception:
                                raise ConnectionError(f'{line_number}: \
Wrong meta-data')

                            if (conct[0].add_next((conct[1], mx_c)) or
                                    conct[1].add_next((conct[0], mx_c))):
                                raise ConnectionError(f'{line_number}: \
Wrong meta-data')
                        else:
                            text_error = f'{line_number}: wrong name for\
 connection'
                            raise HubError(text_error)
                    elif (name_arg[0] == 'nb_drones'):
                        try:
                            count_drons = int(name_arg[1])
                        except Exception:
                            text_error = f'{line_number}: incorrect value'
                            raise HubError(text_error)
                    else:
                        # appropriate error prompt
                        # if nb_drones wasnt defined firstly
                        text_error = f'{line_number}: '
                        if line_number != 1 and name_arg[0] == 'nb_drones':
                            text_error += 'nb_drones wasn`t\
 defined firstly'
                        # if end_hub not only one
                        elif (name_arg[0] == 'end_hub' and
                              data['end_hub'] is not None):
                            text_error += 'end_hub\
  defined twice'
                        elif (name_arg[0] == 'hub' and
                              data['start_hub'] is None):
                            raise HubError(f'{line_number}: \
No start or end hubs at beginning')

                        # same to previos
                        elif (name_arg[0] == 'start_hub' and
                              data['start_hub'] is not None):
                            text_error += 'start_hub\
  defined twice'
                        else:
                            text_error += 'not appropriate config variable\
 definition'
                        raise ConfError(text_error)

        for d in range(count_drons):
            x, y = data['start_hub'].pos
            data['dron'].append(Dron(id=d,
                                     c_pos=(x-1, y-1),
                                     pos=data['start_hub']))

        return data
