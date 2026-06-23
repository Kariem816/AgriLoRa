from fastapi import FastAPI
from handler import Handler
from handler.http import plots
from handler.http import sensors
from handler.http import sensor_data
from service.plots import PlotsService
from service.sensors import SensorsService
from service.sensor_data import SensorDataService
import uvicorn


class HttpHandler(Handler):
    def __init__(self, plots_service: PlotsService, sensors_service: SensorsService, sensor_data_service: SensorDataService):
        app = FastAPI(title="LoRA Server", version="0.1.0")

        plot_handler = plots.PlotHandler(plots_service)
        sensors_handler = sensors.SensorHandler(sensors_service)
        sensor_data_handler = sensor_data.SensorDataHandler(
            sensor_data_service)

        app.include_router(plot_handler.router)
        app.include_router(sensors_handler.router)
        app.include_router(sensor_data_handler.router)

        self.app = app
        self.server = None

    async def start(self) -> None:
        config = uvicorn.Config(
            self.app,
            host="0.0.0.0",
            port=8000,
            loop="asyncio"  # Ensures Uvicorn attaches to the existing loop
        )
        self.server = uvicorn.Server(config)
        await self.server.serve()

    def stop(self) -> None:
        if self.server:
            self.server.should_exit = True
