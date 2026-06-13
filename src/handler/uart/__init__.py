from handler import Handler


class UartHandler(Handler):
    def __init__(self, sensors_service, sensor_data_service):
        # Initialize UART communication here
        pass

    async def start(self) -> None:
        # Start UART communication loop here
        pass

    def stop(self) -> None:
        # Clean up UART communication here
        pass
