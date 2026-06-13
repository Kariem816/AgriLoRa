from typing import Any, AsyncGenerator, Callable
from unittest import result
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import CursorResult, select, delete, update
from db.schema.sensors import Sensor
from domain.schemas import SensorCreate, SensorCreated, SensorResponse

GetDbCallable = Callable[[], AsyncGenerator[AsyncSession, Any]]


class SensorsRepository:
    def __init__(self, gen: GetDbCallable):
        self.gen = gen

    async def create(self, data: SensorCreate) -> SensorCreated:
        gen = self.gen()
        try:
            db = await anext(gen)
            sensor = Sensor(name=data.name)
            db.add(sensor)
            await db.commit()
            await db.refresh(sensor)
            return SensorCreated(id=sensor.id)
        finally:
            await gen.aclose()

    async def get_by_id(self, sensor_id: int) -> SensorResponse | None:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                select(Sensor).where(Sensor.id == sensor_id)
            )
            res = result.scalar_one_or_none()
            if res:
                return SensorResponse(id=res.id, name=res.name)
            return None
        finally:
            await gen.aclose()

    async def get_all(self) -> list[SensorResponse]:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(select(Sensor))
            return [SensorResponse(id=s.id, name=s.name) for s in result.scalars().all()]
        finally:
            await gen.aclose()

    async def update(self, sensor_id: int, name: str) -> bool:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                update(Sensor).where(Sensor.id == sensor_id).values(name=name)
            )
            assert isinstance(result, CursorResult)
            rows = result.rowcount
            if rows == 0:
                return False
            await db.commit()
            return True
        finally:
            await gen.aclose()

    async def delete(self, sensor_id: int) -> bool:
        gen = self.gen()
        try:
            db = await anext(gen)
            res = await db.execute(
                delete(Sensor).where(Sensor.id ==
                                     sensor_id).returning(Sensor.id)
            )
            deleted_id = res.scalar_one_or_none()
            if not deleted_id:
                return False
            await db.commit()
            return True
        finally:
            await gen.aclose()
