from typing import Any, AsyncGenerator, Callable
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import select, desc
from db.schema.sensor_data import SensorData
from domain.schemas import SensorDataCreate, SensorDataCreated, SensorDataRequest, SensorDataResponse
from datetime import datetime, timezone

GetDbCallable = Callable[[], AsyncGenerator[AsyncSession, Any]]


class SensorDataRepository:
    def __init__(self, gen: GetDbCallable):
        self.gen = gen

    async def create(self, data: SensorDataCreate) -> SensorDataCreated:
        sensor_data = SensorData(
            sensor_id=data.sensor_id,
            value=data.value,
            timestamp=datetime.now(timezone.utc)
        )
        gen = self.gen()
        try:
            db = await anext(gen)
            db.add(sensor_data)
            await db.commit()
            await db.refresh(sensor_data)
            return SensorDataCreated(id=sensor_data.id)
        finally:
            await gen.aclose()

    async def get_last_n(self, sensor_id: int, limit: int = 100) -> list[SensorDataResponse]:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                select(SensorData)
                .where(SensorData.sensor_id == sensor_id)
                .order_by(desc(SensorData.timestamp))
                .limit(limit)
            )
            res = result.scalars().all()
            mapped = [SensorDataResponse(
                sensor_id=d.sensor_id,
                timestamp=d.timestamp,
                value=d.value
            ) for d in res]
            return mapped
        finally:
            await gen.aclose()
            
    async def get_n_after(self, data: SensorDataRequest) -> list[SensorDataResponse]:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                select(SensorData)
                .where(SensorData.sensor_id == data.sensor_id and SensorData.timestamp > data.after)
                .order_by(desc(SensorData.timestamp))
                .limit(data.limit)
            )
            res = result.scalars().all()
            mapped = [SensorDataResponse(
                sensor_id=d.sensor_id,
                timestamp=d.timestamp,
                value=d.value
            ) for d in res]
            return mapped
        finally:
            await gen.aclose()
