from fastapi import APIRouter, HTTPException
from service.sensors import SensorsService
from domain.schemas import SensorCreate, SensorUpdate, SensorResponse


class SensorHandler:
    def __init__(self, sensor_service: SensorsService):
        self.service = sensor_service
        self.router = APIRouter(prefix="/sensors", tags=["sensors"])

        self._register_routes()

    def _register_routes(self):
        @self.router.post("/", response_model=SensorResponse)
        async def create_sensor(sensor: SensorCreate):
            return await self.service.create_sensor(sensor)

        @self.router.get("/{sensor_id}", response_model=SensorResponse)
        async def get_sensor(sensor_id: int):
            result = await self.service.get_sensor(sensor_id)
            if not result:
                raise HTTPException(status_code=404, detail="Sensor not found")
            return result

        @self.router.get("/", response_model=list[SensorResponse])
        async def get_all_sensors():
            return await self.service.get_all_sensors()

        @self.router.put("/{sensor_id}", response_model=SensorResponse)
        async def update_sensor(sensor_id: int, sensor: SensorUpdate):
            result = await self.service.update_sensor(sensor_id, sensor)
            if not result:
                raise HTTPException(status_code=404, detail="Sensor not found")
            return result

        @self.router.delete("/{sensor_id}")
        async def delete_sensor(sensor_id: int):
            success = await self.service.delete_sensor(sensor_id)
            if not success:
                raise HTTPException(status_code=404, detail="Sensor not found")
            return {"message": "Sensor deleted"}
