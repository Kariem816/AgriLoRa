from server.src.repo.sensors import SensorsRepository
from server.src.domain.schemas import SensorCreate, SensorCreated, SensorUpdate, SensorResponse


class SensorsService:
    def __init__(self, repo: SensorsRepository):
        self.repo = repo

    async def create_sensor(self, sensor: SensorCreate) -> SensorCreated:
        return await self.repo.create(sensor)

    async def get_sensor(self, sensor_id: int) -> SensorResponse | None:
        return await self.repo.get_by_id(sensor_id)

    async def get_all_sensors(self) -> list[SensorResponse]:
        return await self.repo.get_all()

    async def update_sensor(self, sensor_id: int, sensor: SensorUpdate) -> bool:
        return await self.repo.update(sensor_id, sensor.name)

    async def delete_sensor(self, sensor_id: int) -> bool:
        return await self.repo.delete(sensor_id)
