from pydantic import BaseModel
from datetime import datetime
from db.schema.sensor_type import SensorType

class PlotCreate(BaseModel):
    name: str
    
class PlotCreated(BaseModel):
    id: int
    
class PlotUpdate(BaseModel):
    name: str
    
class PlotResponse(BaseModel):
    id: int
    name: str

class SensorCreate(BaseModel):
    name: str
    type: SensorType
    plot_id: int
    loc_x: float
    loc_y: float


class SensorCreated(BaseModel):
    id: int


# TODO: split into multiple schemas
class SensorUpdate(BaseModel):
    name: str
    type: SensorType
    plot_id: int
    loc_x: float
    loc_y: float


class SensorResponse(BaseModel):
    id: int
    name: str
    type: SensorType
    plot_id: int
    plot_name: str
    loc_x: float
    loc_y: float


class SensorBulkResponse(BaseModel):
    id: int
    name: str
    type: SensorType
    plot_id: int
    plot_name: str

class SensorDataCreate(BaseModel):
    sensor_id: int
    value: float

class SensorDataRequest(BaseModel):
    sensor_id: int
    after: datetime
    limit: int = 100

class SensorDataCreated(BaseModel):
    id: int


class SensorDataResponse(BaseModel):
    sensor_id: int
    timestamp: datetime
    value: float
