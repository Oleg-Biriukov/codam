from enum import Enum
from pydantic import Field, BaseModel, model_validator
from datetime import datetime


class Rank(Enum):
    cadet = 'cadet'
    officer = 'officer'
    lieutenant = 'lieutenant'
    captain = 'captain'
    commander = 'commander'


class CrewMember(BaseModel):
    member_id: str = Field(min_length=2, max_length=10)
    name: str = Field(min_length=2, max_length=50)
    rank: Rank
    age: int = Field(ge=18, le=80)
    specialization: str = Field(min_length=2, max_length=30)
    years_experience: int = Field(ge=0, le=50)
    is_active: bool = True


class SpaceMission(BaseModel):
    mission_id: str = Field(min_length=5, max_length=15)
    mission_name: str = Field(min_length=3, max_length=100)
    destination: str = Field(min_length=3, max_length=50)
    launch_date: datetime
    duration_days: int = Field(ge=1, le=3650)
    crew: list[CrewMember] = Field(min_length=1, max_length=12)
    mission_status: str = "planned"
    budget_millions: float = Field(ge=1.0, le=10000.0)

    @model_validator(mode='after')
    def mission_validatio(self):
        errors: list[str] = []
        if self.mission_id[:1:] != 'M':
            errors.append('mission ID must start with "M"')

        if len([m for m in self.crew
                if m.rank == Rank.captain or m.rank == Rank.commander]) == 0:
            errors.append("must have at least one Commander or Captain")

        if self.duration_days > 365:
            len_c: int = len([m for m in self.crew if m.years_experience <= 5])
            if len(self.crew) / 2 <= len_c:
                errors.append('long missions (> 365 days) need 50% \
experienced crew (5+ years)')

        if len([m for m in self.crew
                if m.is_active is False]) > 0:
            errors.append('all crew members must be active')
        if errors:
            raise ValueError(errors)
        return self


def main():
    try:
        import json
        with open('space_missions.json', 'r') as s_m:
            data = json.load(s_m)[0]
        crews = [CrewMember(**c) for c in data['crew']]
        data['crew'] = crews
        mission = SpaceMission(**data)
        print(f'''Space Mission Crew Validation
=========================================
Valid mission created:
Mission: {mission.mission_name}
ID: {mission.mission_id}
Destination: {mission.destination}
Duration: {mission.duration_days}
Budget: ${mission.budget_millions}
Crew size: {len(mission.crew)}
Crew members:''')
        for mem in crews:
            print(f'- {mem.name} ({mem.rank.value}) - {mem.specialization}')
    except Exception as e:
        for error in e.errors():
            print(error['msg'])


if __name__ == "__main__":
    main()
