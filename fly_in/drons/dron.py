from pydantic import BaseModel, PrivateAttr, Field
from hubs.hub import Hub


class Dron(BaseModel):
    id: int = Field(ge=0, le=10000)
    _pos: Hub = PrivateAttr()

    def move_to(self, hub: Hub) -> bool:
        if hub.max_drones > 0 and hub.zone != 'blocked':
            self._pos = hub
            return True
        return False
