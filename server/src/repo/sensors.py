from typing import Any, AsyncGenerator, Callable
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import CursorResult, select, delete, update
from sqlalchemy.orm import selectinload
from db.schema import Sensor
from domain.schemas import SensorBulkResponse, SensorCreate, SensorCreated, SensorResponse, SensorUpdate

GetDbCallable = Callable[[], AsyncGenerator[AsyncSession, Any]]


class SensorsRepository:
    def __init__(self, gen: GetDbCallable):
        self.gen = gen

    async def create(self, data: SensorCreate) -> SensorCreated:
        gen = self.gen()
        try:
            db = await anext(gen)
            sensor = Sensor(
                name=data.name,
                type=data.type,
                plot_id=data.plot_id,
                loc_x=data.loc_x,
                loc_y=data.loc_y)
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
                select(Sensor)
                .options(selectinload(Sensor.plot))
                .where(Sensor.id == sensor_id)
            )
            res = result.scalar_one_or_none()
            if res:
                return SensorResponse(
                    id=res.id,
                    name=res.name,
                    type=res.type,
                    plot_id=res.plot.id,
                    plot_name=res.plot.name,
                    loc_x=res.loc_x,
                    loc_y=res.loc_y)
            return None
        finally:
            await gen.aclose()

    async def get_all(self, page: int, limit: int) -> list[SensorBulkResponse]:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(select(Sensor)
                                      .options(selectinload(Sensor.plot))
                                      .limit(limit).offset((page - 1) * limit))
            return [SensorBulkResponse(
                id=s.id,
                name=s.name,
                type=s.type,
                plot_id=s.plot.id,
                plot_name=s.plot.name) for s in result.scalars().all()]
        finally:
            await gen.aclose()

    async def update(self, sensor_id: int, data: SensorUpdate) -> bool:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                update(Sensor).where(Sensor.id == sensor_id).values(
                    name=data.name,
                    type=data.type,
                    plot_id=data.plot_id,
                    loc_x=data.loc_x,
                    loc_y=data.loc_y)
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
