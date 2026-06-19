from sqlalchemy import Integer, String
from sqlalchemy.orm import Mapped, mapped_column, relationship
from ..db import Base


class Plot(Base):
    __tablename__ = "plots"

    id: Mapped[int] = mapped_column(
        Integer, primary_key=True, autoincrement=True)
    name: Mapped[str] = mapped_column(String, nullable=False)
    sensors: Mapped[list["Sensor"]] = relationship(back_populates="plot", cascade="all, delete-orphan")
