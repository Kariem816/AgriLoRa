idf_component_register(SRCS "main.c"
                    REQUIRES agri frame
                    PRIV_REQUIRES esp_driver_gpio esp_driver_spi esp_driver_uart
)
