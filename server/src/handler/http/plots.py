from fastapi import APIRouter, HTTPException
from service.plots import PlotsService
from domain.schemas import PlotCreate, PlotCreated, PlotResponse, PlotUpdate, SensorResponse

class PlotHandler:
    def __init__(self, plot_service: PlotsService):
        self.service = plot_service
        self.router = APIRouter(prefix="/plots", tags=["plots"])

        self._register_routes()

    def _register_routes(self):
        @self.router.post("/", response_model=PlotCreated)
        async def create_plot(plot: PlotCreate):
            return await self.service.create_plot(plot)

        @self.router.get("/{plot_id}", response_model=PlotResponse)
        async def get_plot(plot_id: int):
            result = await self.service.get_plot(plot_id)
            if not result:
                raise HTTPException(status_code=404, detail="Plot not found")
            return result

        @self.router.get("/", response_model=list[PlotResponse])
        async def get_all_plots():
            return await self.service.get_all_plots()
        
        @self.router.get("/{plot_id}/sensors", response_model=list[SensorResponse])
        async def get_plot_sensors(plot_id: int) -> list[SensorResponse]:
            return await self.service.get_plot_sensors(plot_id)

        @self.router.put("/{plot_id}")
        async def update_plot(plot_id: int, plot: PlotUpdate):
            result = await self.service.update_plot(plot_id, plot)
            if not result:
                raise HTTPException(status_code=404, detail="Plot not found")
            return {"message": "Plot updated"}

        @self.router.delete("/{plot_id}")
        async def delete_plot(plot_id: int):
            success = await self.service.delete_plot(plot_id)
            if not success:
                raise HTTPException(status_code=404, detail="Plot not found")
            return {"message": "Plot deleted"}