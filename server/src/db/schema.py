from sqlalchemy import Integer, String, ForeignKey, Float, DateTime, Enum
from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column, relationship
from datetime import datetime
import enum

class SensorType(enum.Enum):
    HUMIDITY = "humidity"

class Base(DeclarativeBase):
    pass

class Plot(Base):
    __tablename__ = "plots"

    id: Mapped[int] = mapped_column(
        Integer, primary_key=True, autoincrement=True)
    name: Mapped[str] = mapped_column(String, nullable=False)

    width: Mapped[float] = mapped_column(Float, nullable=False, default=23)
    height: Mapped[float] = mapped_column(Float, nullable=False, default=6)

    sensors: Mapped[list["Sensor"]] = relationship(back_populates="plot", cascade="all, delete-orphan")

class Sensor(Base):
    __tablename__ = "sensors"

    id: Mapped[int] = mapped_column(
        Integer, primary_key=True, autoincrement=True)
    name: Mapped[str] = mapped_column(String, nullable=False)

    loc_x: Mapped[float] = mapped_column(Float, nullable=False)
    loc_y: Mapped[float] = mapped_column(Float, nullable=False)

    plot_id: Mapped[int] = mapped_column(ForeignKey("plots.id"))
    plot: Mapped["Plot"] = relationship(back_populates="sensors")

    type: Mapped[SensorType] = mapped_column(Enum(SensorType, values_callable=lambda obj: [e.value for e in obj]), nullable=False) # type: ignore



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
