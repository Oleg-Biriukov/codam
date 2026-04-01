from pydantic import BaseModel, PrivateAttr, Field
from hubs.hub import Hub
from typing import Self


class Dron(BaseModel):
    id: int = Field(ge=0, le=10000)
    pos: Hub
    occupation: dict = {}

    def move_to(self, hub: Hub) -> bool:
        if (hub.max_drones > 0 and hub.zone != 'blocked' and
                hub.max_link_capacity > 0):
            self.pos.max_drones += 1
            hub.max_drones -= 1
            hub.max_link_capacity -= 1
            self.pos = hub
            return True
        return False
