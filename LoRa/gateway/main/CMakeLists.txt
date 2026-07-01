idf_component_register(SRCS "gateway.c"
                    INCLUDE_DIRS "."
                    REQUIRES packets
                    PRIV_REQUIRES esp_driver_gpio esp_driver_spi esp_driver_uart
)

# target_compile_definitions(${COMPONENT_LIB} PRIVATE DISABLE_LOGS)
