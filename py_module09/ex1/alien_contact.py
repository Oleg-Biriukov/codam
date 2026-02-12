from enum import Enum
from pydantic import Field, BaseModel, model_validator
from datetime import datetime


class ContactType(Enum):
    radio = 'radio'
    visual = 'visual'
    physical = 'physical'
    telepathic = 'telepathic'


class AlienContact(BaseModel):
    contact_id: str = Field(min_length=5, max_length=25)
    timestamp: datetime
    location: str = Field(min_length=3, max_length=100)
    contact_type: ContactType
    signal_strength: float = Field(ge=0.0, le=10.0)
    duration_minutes: int = Field(ge=1, le=1440)
    witness_count: int = Field(ge=1, le=100)
    message_received: str | None = Field(default=None, max_length=500)
    is_verified: bool = False

    @model_validator(mode="after")
    def custom_validation(self):
        if self.contact_id[:1] != 'AC':
            raise ValueError('contact ID must start with "AC"')
        elif self.is_verified is False:
            raise ValueError('physical contact reports must be verified')
        elif self.contact_type == ContactType.telepathic:
            if self.witness_count < 3:
                raise ValueError('telepathic contact requires\
 at least 3 witnesses')
        elif self.signal_strength > 7.0:
            if self.message_received is None:
                raise ValueError('strong signals (> 7.0) should\
 includereceived messages')
        return self


def main():
    radio_c = AlienContact(contact_id='AC_2024_001',
                           contact_type='radio',
                           location='Area 51, Nevada',
                           signal_strength=8.5,
                           duration_minutes=45,
                           witness_count=5,
                           message_received='Greetings from Zeta Reticuli',
                           is_verified=True,
                           timestamp=datetime.today())

    print(f'''Alien Contact Log Validation
======================================
Valid contact report:
ID: {radio_c.contact_id}
Type: {radio_c.contact_type}
Location: {radio_c.location}
Signal: {radio_c.signal_strength}/10
Duration: {radio_c.duration_minutes} minutes
Witnesses: {radio_c.witness_count}
Message: {radio_c.message_received}''')


if __name__ == "__main__":
    main()
