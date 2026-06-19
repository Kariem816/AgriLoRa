import serial_asyncio
from handler import Handler


class UartHandler(Handler):
    def __init__(self, sensors_service, sensor_data_service):
        self.sensors_service = sensors_service
        self.sensor_data_service = sensor_data_service

        self.running = False

    async def start(self) -> None:
        reader, writer = await serial_asyncio.open_serial_connection(
            url='COM3',  # Replace with your actual COM port
            baudrate=9600
        )

        while self.running:
            # Read data from the UART
            data = await reader.readline()
            if data:
                # Process the received data
                print(f"Received: {data.decode('utf-8').strip()}")
                # Here you can call your sensor_data_service to handle the data

        writer.close()
        await writer.wait_closed()

    def stop(self) -> None:
        self.running = False
