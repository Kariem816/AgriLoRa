from fastapi import APIRouter
from service.sensor_data import SensorDataService
from domain.schemas import SensorDataCreate, SensorDataCreated, SensorDataResponse
from datetime import datetime


class SensorDataHandler:
    def __init__(self, sensor_data_service: SensorDataService):
        self.service = sensor_data_service
        self.router = APIRouter(prefix="/sensor-data", tags=["sensor-data"])

        self._register_routes()

    def _register_routes(self):
        @self.router.post("/", response_model=SensorDataCreated)
        async def add_sensor_data(data: SensorDataCreate):
            return await self.service.add_data(data)

        @self.router.get("/{sensor_id}", response_model=list[SensorDataResponse])
        async def get_sensor_data(sensor_id: int, limit: int = 100, after: datetime | None = None):
            return await self.service.get_data(sensor_id, limit, after)
