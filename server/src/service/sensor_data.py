from datetime import datetime

from repo.sensor_data import SensorDataRepository
from domain.schemas import SensorDataCreate, SensorDataCreated, SensorDataResponse, SensorDataRequest


class SensorDataService:
    def __init__(self, repo: SensorDataRepository):
        self.repo = repo

    async def add_data(self, data: SensorDataCreate) -> SensorDataCreated:
        return await self.repo.create(data)

    async def get_data(self, sensor_id: int, limit: int, after: datetime | None) -> list[SensorDataResponse]:
        if not after:
            return await self.repo.get_last_n(sensor_id, limit)
        return await self.repo.get_n_after(SensorDataRequest(
            sensor_id=sensor_id,
            limit=limit,
            after=after))
            
