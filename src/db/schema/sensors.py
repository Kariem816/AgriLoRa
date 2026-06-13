from sqlalchemy import Integer, String
from sqlalchemy.orm import Mapped, mapped_column
from db.db import Base


class Sensor(Base):
    __tablename__ = "sensors"

    id: Mapped[int] = mapped_column(
        Integer, primary_key=True, autoincrement=True)
    name: Mapped[str] = mapped_column(String, nullable=False)
