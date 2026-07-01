idf_component_register(SRCS "node.c"
                    INCLUDE_DIRS "."
                    REQUIRES agri
                    PRIV_REQUIRES esp_driver_gpio esp_driver_spi)
