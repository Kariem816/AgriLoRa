    
from repo.plots import PlotsRepository
from domain.schemas import PlotCreate, PlotCreated, PlotResponse, PlotUpdate, SensorResponse


class PlotsService:
    def __init__(self, repo: PlotsRepository):
        self.repo = repo

    async def create_plot(self, plot: PlotCreate) -> PlotCreated:
        return await self.repo.create(plot)

    async def get_plot(self, plot_id: int) -> PlotResponse | None:
        return await self.repo.get_by_id(plot_id)

    async def get_plot_sensors(self, plot_id: int) -> list[SensorResponse]:
        return await self.repo.get_sensors(plot_id)

    async def get_all_plots(self) -> list[PlotResponse]:
        return await self.repo.get_all()

    async def update_plot(self, plot_id: int, plot: PlotUpdate) -> bool:
        return await self.repo.update(plot_id, plot)

    async def delete_plot(self, plot_id: int) -> bool:
        return await self.repo.delete(plot_id)