from typing import Any, AsyncGenerator, Callable
from sqlalchemy.ext.asyncio import AsyncSession
from sqlalchemy import CursorResult, select, delete, update
from db.schema import Plot, Sensor
from domain.schemas import PlotCreate, PlotCreated, PlotUpdate, PlotResponse, SensorResponse

GetDbCallable = Callable[[], AsyncGenerator[AsyncSession, Any]]

class PlotsRepository:
    def __init__(self, gen: GetDbCallable):
        self.gen = gen

    async def create(self, data: PlotCreate) -> PlotCreated:
        gen = self.gen()
        try:
            db = await anext(gen)
            plot = Plot(name=data.name)
            db.add(plot)
            await db.commit()
            await db.refresh(plot)
            return PlotCreated(id=plot.id)
        finally:
            await gen.aclose()

    async def get_by_id(self, plot_id: int) -> PlotResponse | None:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                select(Plot).where(Plot.id == plot_id)
            )
            res = result.scalar_one_or_none()
            if res:
                return PlotResponse(id=res.id, name=res.name)
            return None
        finally:
            await gen.aclose()
            
    async def get_sensors(self, plot_id: int) -> list[SensorResponse]:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(select(Sensor).where(Sensor.plot_id == plot_id))
            return [SensorResponse(
                id=s.id,
                name=s.name,
                type=s.type,
                plot_id=s.plot.id,
                plot_name=s.plot.name,
                loc_x=s.loc_x,
                loc_y=s.loc_y
                ) for s in result.scalars().all()]
        finally:
            await gen.aclose()

    async def get_all(self) -> list[PlotResponse]:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(select(Plot))
            return [PlotResponse(id=s.id, name=s.name) for s in result.scalars().all()]
        finally:
            await gen.aclose()

    async def update(self, plot_id: int, data: PlotUpdate) -> bool:
        gen = self.gen()
        try:
            db = await anext(gen)
            result = await db.execute(
                update(Plot).where(Plot.id == plot_id).values(name=data.name)
            )
            assert isinstance(result, CursorResult)
            rows = result.rowcount
            if rows == 0:
                return False
            await db.commit()
            return True
        finally:
            await gen.aclose()

    async def delete(self, plot_id: int) -> bool:
        gen = self.gen()
        try:
            db = await anext(gen)
            res = await db.execute(
                delete(Plot).where(Plot.id ==
                                     plot_id).returning(Plot.id)
            )
            deleted_id = res.scalar_one_or_none()
            if not deleted_id:
                return False
            await db.commit()
            return True
        finally:
            await gen.aclose()
