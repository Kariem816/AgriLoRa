from abc import ABC, abstractmethod


class Handler(ABC):
    @abstractmethod
    async def start(self) -> None:
        pass

    @abstractmethod
    def stop(self) -> None:
        pass
