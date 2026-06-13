import os
import signal
import sys
import asyncio
import ctypes
from typing import Callable

# Database
from db.db import get_db, engine, Base

# Repositories
from repo.sensors import SensorsRepository
from repo.sensor_data import SensorDataRepository

# Services
from service.sensors import SensorsService
from service.sensor_data import SensorDataService

# Handlers
from handler import Handler
from handler.http import HttpHandler
from handler.uart import UartHandler


_win_ctrl_handler_ref = None  # Module-level keep-alive


def register_signal_handler(loop: asyncio.AbstractEventLoop, shutdown: Callable[[], None]) -> None:
    if sys.platform == "win32":
        global _win_ctrl_handler_ref

        def win_ctrl_handler(dw_ctrl_type):
            if dw_ctrl_type in (0, 2):  # CTRL_C_EVENT or CTRL_CLOSE_EVENT
                loop.call_soon_threadsafe(shutdown)
                return True
            return False

        WINFUNCTYPE = ctypes.WINFUNCTYPE(ctypes.c_bool, ctypes.c_ulong)
        handler_ptr = WINFUNCTYPE(win_ctrl_handler)
        ctypes.windll.kernel32.SetConsoleCtrlHandler(handler_ptr, True)
        _win_ctrl_handler_ref = handler_ptr  # Survives function return
    else:
        for sig in (signal.SIGINT, signal.SIGTERM):
            loop.add_signal_handler(sig, shutdown)


async def main():
    # Migrate the database (if needed)
    async with engine.begin() as conn:
        await conn.run_sync(Base.metadata.create_all)

    # Initialize repositories
    sensors_repo = SensorsRepository(get_db)
    sensor_data_repo = SensorDataRepository(get_db)

    # Initialize services
    sensors_service = SensorsService(sensors_repo)
    sensor_data_service = SensorDataService(sensor_data_repo)

    # Instantiate handlers
    http_handler = HttpHandler(sensors_service, sensor_data_service)
    uart_handler = UartHandler(sensors_service, sensor_data_service)

    # Determine which handler to initialize
    handler_type = os.getenv("HANDLER_TYPE", "http").lower()
    print(f"Starting IoT server with handler type: {handler_type}")
    tasks = []
    handlers: list[Handler] = []
    if handler_type in ("http", "both"):
        handlers.append(http_handler)
        tasks.append(http_handler.start())
    if handler_type in ("uart", "both"):
        handlers.append(uart_handler)
        tasks.append(uart_handler.start())

    loop = asyncio.get_running_loop()

    def shutdown():
        print("\nShutting down IoT handlers...")
        for handler in handlers:
            handler.stop()

    register_signal_handler(loop, shutdown)

    await asyncio.gather(*tasks)


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        pass
