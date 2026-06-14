from server.src.repo.sensor_data import SensorDataRepository
from server.src.domain.schemas import SensorDataCreate, SensorDataCreated, SensorDataResponse


class SensorDataService:
    def __init__(self, repo: SensorDataRepository):
        self.repo = repo

    async def add_sensor_data(self, data: SensorDataCreate) -> SensorDataCreated:
        return await self.repo.create(data)

    async def get_last_100(self, sensor_id: int) -> list[SensorDataResponse]:
        return await self.repo.get_last_n(sensor_id, limit=100)
