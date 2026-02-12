from pydantic import Field, BaseModel
from datetime import datetime
from typing import Optional


class SpaceStation(BaseModel):
    station_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=1, max_length=50)
    crew_size: int = Field(ge=1, le=20)
    power_level: float = Field(ge=0.0, le=100.0)
    oxygen_level: float = Field(ge=0.0, le=100.0)
    last_maintenance: datetime
    is_operational: bool
    notes: Optional[str] = Field(default=None, max_length=200)


def main():
    station = SpaceStation(station_id='ISS001',
                           name='International Space Station',
                           crew_size=6, power_level=85.5,
                           oxygen_level=92.3, is_operational=True,
                           last_maintenance=datetime.today()
                           )
    print(f'''Space Station Data Validation
========================================
Valid station created:
ID: {station.station_id}
Name: {station.name}
Crew: {station.crew_size} people
Power: {station.power_level}%
Oxygen: {station.oxygen_level}%
Status: {station.is_operational}''')
    try:
        station = SpaceStation(station_id='ISS001',
                               name='International Space Station',
                               crew_size=40, power_level=85.5,
                               oxygen_level=92.3, is_operational=True,
                               last_maintenance=datetime.today()
                               )
    except Exception as e:
        print('''========================================
Expected validation error:''', e)


if __name__ == "__main__":
    main()
