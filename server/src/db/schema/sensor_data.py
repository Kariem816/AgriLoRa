from sqlalchemy import Float, ForeignKey, Integer, DateTime
from sqlalchemy.orm import Mapped, mapped_column
from ..db import Base
from datetime import datetime


class SensorData(Base):
    __tablename__ = "sensor_data"

    id: Mapped[int] = mapped_column(
        Integer, primary_key=True, autoincrement=True)
    sensor_id: Mapped[int] = mapped_column(
        ForeignKey("sensors.id"), nullable=False, index=True)
    timestamp: Mapped[datetime] = mapped_column(
        DateTime(timezone=True), nullable=False, index=True)
    value: Mapped[float] = mapped_column(
        Float, nullable=False)
