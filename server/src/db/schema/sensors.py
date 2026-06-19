from sqlalchemy import Float, Integer, String, Enum, ForeignKey
from sqlalchemy.orm import Mapped, mapped_column, relationship 
from ..db import Base

from .sensor_type import SensorType

class Sensor(Base):
    __tablename__ = "sensors"

    id: Mapped[int] = mapped_column(
        Integer, primary_key=True, autoincrement=True)
    name: Mapped[str] = mapped_column(String, nullable=False)

    loc_x: Mapped[float] = mapped_column(Float, nullable=False)
    loc_y: Mapped[float] = mapped_column(Float, nullable=False)

    plot_id: Mapped[int] = mapped_column(ForeignKey("plots.id"))
    plot: Mapped["Plot"] = relationship(back_populates="sensors")

    type: Mapped[SensorType] = mapped_column(Enum(SensorType, values_callable=lambda obj: [e.value for e in obj]), nullable=False)