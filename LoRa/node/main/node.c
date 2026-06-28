#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "sx127x.h"

static const char *TAG = "LoRa-TX";

#define PIN_LORA_NSS  5
#define PIN_LORA_RST  14
#define PIN_LORA_SCK  18
#define PIN_LORA_MISO 19
#define PIN_LORA_MOSI 23
#define PIN_LORA_DIO  26

#define LORA_SPI_HOST    SPI2_HOST
#define LORA_FREQUENCY   433000000UL
#define LORA_BANDWIDTH   SX127X_BW_125000

#define TX_INTERVAL_MS   2000

const UBaseType_t txDoneArrayIndex = 0;

static TaskHandle_t  tx_task_handle = NULL;
static sx127x        device;

static TaskHandle_t lora_event_processor_task_handle = NULL;
const UBaseType_t   eventArrayIndex = 0;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(
        lora_event_processor_task_handle,
        eventArrayIndex,
        &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void lora_event_processor_task(void *arg)
{
    while (1) {
        if (ulTaskNotifyTakeIndexed(eventArrayIndex, pdTRUE, portMAX_DELAY) > 0) {
            sx127x_handle_interrupt(&device);
        }
    }
}

static void lora_tx_callback(void *ctx)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveIndexedFromISR(
        tx_task_handle,
        txDoneArrayIndex,
        &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void tx_task(void *arg)
{
    uint32_t counter = 0;
    char     payload[32];
    uint8_t  len;

    while (1) {
        len = (uint8_t)snprintf(payload, sizeof(payload), "PKT #%04lu", counter++);

        ESP_LOGI(TAG, "Sending: %s", payload);

        esp_err_t err = sx127x_lora_tx_set_for_transmission(
                               (uint8_t *)payload, len, &device);
        if (err != SX127X_OK) {
            ESP_LOGE(TAG, "set_for_transmission failed: %d", err);
            goto next;
        }

        err = sx127x_set_opmod(SX127X_MODE_TX, SX127X_MODULATION_LORA, &device);
        if (err != SX127X_OK) {
            ESP_LOGE(TAG, "set TX mode failed: %d", err);
            goto next;
        }

        if (ulTaskNotifyTakeIndexed(txDoneArrayIndex, pdTRUE, pdMS_TO_TICKS(2000)) == 0) {
            ESP_LOGW(TAG, "TxDone timeout!");
        } else {
            ESP_LOGI(TAG, "TxDone OK");
        }

next:
        vTaskDelay(pdMS_TO_TICKS(TX_INTERVAL_MS));
    }
}

void app_main(void)
{
    spi_bus_config_t spi_cfg = {
        .mosi_io_num   = PIN_LORA_MOSI,
        .miso_io_num   = PIN_LORA_MISO,
        .sclk_io_num   = PIN_LORA_SCK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LORA_SPI_HOST, &spi_cfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t spi_dev_cfg = {
        .clock_speed_hz = 9000000,
        .mode           = 0,
        .spics_io_num   = PIN_LORA_NSS,
        .queue_size     = 1,
    };
    spi_device_handle_t spi_device;
    ESP_ERROR_CHECK(spi_bus_add_device(LORA_SPI_HOST, &spi_dev_cfg, &spi_device));

    ESP_ERROR_CHECK(sx127x_create(spi_device, &device));
    ESP_ERROR_CHECK(sx127x_set_opmod(SX127X_MODE_SLEEP, SX127X_MODULATION_LORA, &device));
    ESP_ERROR_CHECK(sx127x_set_frequency(LORA_FREQUENCY, &device));
    ESP_ERROR_CHECK(sx127x_lora_set_bandwidth(LORA_BANDWIDTH, &device));
    ESP_ERROR_CHECK(sx127x_lora_set_implicit_header(NULL, &device));
    ESP_ERROR_CHECK(sx127x_lora_set_spreading_factor(SX127X_SF_9, &device));
    ESP_ERROR_CHECK(sx127x_lora_set_syncword(18, &device));
    ESP_ERROR_CHECK(sx127x_set_preamble_length(8, &device));

    sx127x_tx_set_callback(lora_tx_callback, NULL, &device);

    gpio_install_isr_service(0);
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << PIN_LORA_DIO),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type    = GPIO_INTR_POSEDGE,
    };
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_LORA_DIO, gpio_isr_handler, NULL));

    xTaskCreate(lora_event_processor_task, "lora_evt", 4096, NULL, 10, &lora_event_processor_task_handle);
    xTaskCreate(tx_task,                   "lora_tx",  4096, NULL,  5, &tx_task_handle);

    ESP_LOGI(TAG, "Transmitter started, interval=%dms", TX_INTERVAL_MS);
}