idf_component_register(SRCS "gateway.c"
                    INCLUDE_DIRS "."
                    REQUIRES agri frame
                    PRIV_REQUIRES esp_driver_gpio esp_driver_spi esp_driver_uart
)
