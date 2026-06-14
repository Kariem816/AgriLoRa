from pydantic import BaseModel
from datetime import datetime


class SensorCreate(BaseModel):
    name: str


class SensorCreated(BaseModel):
    id: int


class SensorUpdate(BaseModel):
    name: str


class SensorResponse(BaseModel):
    id: int
    name: str


class SensorDataCreate(BaseModel):
    sensor_id: int
    value: float


class SensorDataCreated(BaseModel):
    id: int


class SensorDataResponse(BaseModel):
    sensor_id: int
    timestamp: datetime
    value: float
