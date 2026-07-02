#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "sx127x.h"

#include <stdio.h>
#include <string.h>

#include "frame.h"
#include "report.h"

#define PIN_LORA_NSS 5
#define PIN_LORA_RST 14
#define PIN_LORA_SCK 18
#define PIN_LORA_MISO 19
#define PIN_LORA_MOSI 23
#define PIN_LORA_DIO 26

#define LORA_SPI_HOST SPI2_HOST
#define LORA_FREQUENCY 433000000UL
#define LORA_BANDWIDTH SX127X_BW_125000

#define UART_PORT UART_NUM_0
#define UART_BAUD 115200
#define UART_BUF_SIZE 1024

#define FRAME_QUEUE_DEPTH 16
#define TX_QUEUE_DEPTH 64

const UBaseType_t eventArrayIndex = 0;
const UBaseType_t txDoneArrayIndex = 0;

static QueueHandle_t frame_queue = NULL;
static QueueHandle_t tx_queue = NULL;

static TaskHandle_t lora_event_processor_task_handle = NULL;
static TaskHandle_t tx_task_handle = NULL;

static sx127x device;

bool log_info(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Frame frame = frame_vlog(FRAME_INFO, fmt, args);
  va_end(args);

  return xQueueSend(frame_queue, &frame, pdMS_TO_TICKS(100));
}

bool log_warn(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Frame frame = frame_vlog(FRAME_WARN, fmt, args);
  va_end(args);

  return xQueueSend(frame_queue, &frame, pdMS_TO_TICKS(100));
}

bool log_err(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Frame frame = frame_vlog(FRAME_ERROR, fmt, args);
  va_end(args);

  return xQueueSend(frame_queue, &frame, pdMS_TO_TICKS(100));
}

bool log_packet(const ReportPacket *packet) {
  Frame frame = {0};
  frame.type = FRAME_PKT;
  frame.len = sizeof(ReportPacket);
  memcpy(frame.payload, packet, sizeof(ReportPacket));

  return xQueueSend(frame_queue, &frame, pdMS_TO_TICKS(100));
}

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveIndexedFromISR(lora_event_processor_task_handle,
                                eventArrayIndex, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void lora_event_processor_task(void *arg) {
  while (1) {
    if (ulTaskNotifyTakeIndexed(eventArrayIndex, pdTRUE, portMAX_DELAY) > 0) {
      sx127x_handle_interrupt(&device);
    }
  }
}

static void uart_queuer_task(void *arg) {
  Packet pkt;

  while (1) {
    size_t buffer_size;
    esp_err_t err = uart_get_buffered_data_len(UART_PORT, &buffer_size);
    if (err != ESP_OK) {
      log_err("uart_queuer: failed to get buffered data length");
      continue;
    }

    if (buffer_size < sizeof(pkt)) {
      vTaskDelay(pdMS_TO_TICKS(100));
      continue;
    }

    int n = uart_read_bytes(UART_PORT, &pkt, sizeof(pkt), portMAX_DELAY);
    if (n < 0) {
      log_err("uart_queuer: failed to read bytes from UART");
      continue;
    }

    if (n != sizeof(pkt)) {
      log_warn("uart_queuer: invalid packet. read %d bytes. expected %zu", n,
               sizeof(pkt));
      continue;
    }

    if (!xQueueSend(tx_queue, &pkt, portMAX_DELAY)) {
      log_warn("uart_queuer: tx_queue full, dropping packet");
    }
  }
}

static void tx_task(void *ctx) {
  sx127x *dev = (sx127x *)ctx;
  Packet pkt;

  while (1) {
    if (!xQueueReceive(tx_queue, &pkt, portMAX_DELAY)) {
      continue;
    }

    packet_calculate_checksum(&pkt);

    esp_err_t err = sx127x_lora_tx_set_for_transmission((const uint8_t *)&pkt,
                                                        sizeof(pkt), dev);
    if (err != SX127X_OK) {
      log_err("tx_task: set_for_transmission failed: %d", err);
      continue;
    }

    err = sx127x_set_opmod(SX127X_MODE_TX, SX127X_MODULATION_LORA, dev);
    if (err != SX127X_OK) {
      log_err("tx_task: set TX mode failed: %d", err);
      continue;
    }

    uint32_t notified =
        ulTaskNotifyTakeIndexed(txDoneArrayIndex, pdTRUE, pdMS_TO_TICKS(2000));
    if (notified == 0) {
      log_warn("tx_task: tx-done timeout, re-arming RX");
    } else {
      log_info("tx_task: tx done");
    }

    // Return to RX mode regardless
    sx127x_set_opmod(SX127X_MODE_RX_CONT, SX127X_MODULATION_LORA, dev);
  }
}

static void rx_task(void *arg) {
  Frame frame;
  while (1) {
    if (!xQueueReceive(frame_queue, &frame, portMAX_DELAY)) {
      continue;
    }

    FrameHeader hdr = {
        .magic = FRAME_MAGIC,
        .type = frame.type,
        .len = frame.len,
    };

    uart_write_bytes(UART_PORT, &hdr, sizeof(hdr));

    if (frame.len) {
      uart_write_bytes(UART_PORT, frame.payload, frame.len);
    }
  }
}

static void lora_tx_callback(void *ctx) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveIndexedFromISR(tx_task_handle, txDoneArrayIndex,
                                &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void lora_rx_callback(void *ctx, uint8_t *data, uint16_t data_length) {
  sx127x *dev = (sx127x *)ctx;

  ReportPacket msg;
  esp_err_t err;
  if (data_length != sizeof(msg.packet)) {
    log_warn("rx_callback: invalid length %d", data_length);
    return;
  }

  memcpy(&msg.packet, data, data_length);
  if (!packet_verify_checksum(&msg.packet)) {
    log_warn("rx_callback: invalid checksum");
    return;
  }

  int16_t rssi;
  err = sx127x_rx_get_packet_rssi(dev, &rssi);
  if (err != SX127X_OK) {
    log_warn("rx_callback: failed to get packet RSSI");
    msg.rssi = -1;
  } else {
    msg.rssi = rssi;
  }

  float snr;
  err = sx127x_lora_rx_get_packet_snr(dev, &snr);
  if (err != SX127X_OK) {
    log_warn("rx_callback: failed to get packet SNR");
    msg.snr = -1.0f;
  } else {
    msg.snr = snr;
  }

  int32_t frequency_error;
  err = sx127x_rx_get_frequency_error(dev, &frequency_error);
  if (err != SX127X_OK) {
    log_warn("rx_callback: failed to get frequency error");
    msg.frequency_error = -1;
  } else {
    msg.frequency_error = frequency_error;
  }

  if (!log_packet(&msg)) {
    log_warn("rx_callback: rx_queue full, dropping packet");
  }
}

void app_main(void) {
  frame_queue = xQueueCreate(FRAME_QUEUE_DEPTH, sizeof(Frame));
  tx_queue = xQueueCreate(TX_QUEUE_DEPTH, sizeof(Packet));
  configASSERT(frame_queue && tx_queue);

  uart_config_t uart_cfg = {
      .baud_rate = UART_BAUD,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  };
  ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_cfg));
  ESP_ERROR_CHECK(uart_driver_install(UART_PORT, UART_BUF_SIZE, 0, 0, NULL, 0));

  spi_bus_config_t spi_cfg = {
      .mosi_io_num = PIN_LORA_MOSI,
      .miso_io_num = PIN_LORA_MISO,
      .sclk_io_num = PIN_LORA_SCK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 0,
  };
  ESP_ERROR_CHECK(spi_bus_initialize(LORA_SPI_HOST, &spi_cfg, SPI_DMA_CH_AUTO));

  spi_device_interface_config_t spi_dev_cfg = {
      .clock_speed_hz = 4e6,
      .spics_io_num = PIN_LORA_NSS,
      .queue_size = 16,
      .command_bits = 0,
      .address_bits = 8,
      .dummy_bits = 0,
      .mode = 0,
  };
  spi_device_handle_t spi_device;
  ESP_ERROR_CHECK(spi_bus_add_device(LORA_SPI_HOST, &spi_dev_cfg, &spi_device));

  ESP_ERROR_CHECK(sx127x_create(spi_device, &device));
  ESP_ERROR_CHECK(
      sx127x_set_opmod(SX127X_MODE_STANDBY, SX127X_MODULATION_LORA, &device));
  ESP_ERROR_CHECK(sx127x_set_frequency(LORA_FREQUENCY, &device));
  ESP_ERROR_CHECK(sx127x_lora_reset_fifo(&device));
  ESP_ERROR_CHECK(sx127x_lora_set_bandwidth(LORA_BANDWIDTH, &device));
  ESP_ERROR_CHECK(sx127x_lora_set_implicit_header(NULL, &device));
  ESP_ERROR_CHECK(sx127x_lora_set_spreading_factor(SX127X_SF_9, &device));
  ESP_ERROR_CHECK(sx127x_lora_set_syncword(18, &device));
  ESP_ERROR_CHECK(sx127x_set_preamble_length(8, &device));
  sx127x_tx_header_t header = {.enable_crc = true,
                               .coding_rate = SX127X_CR_4_5};
  ESP_ERROR_CHECK(sx127x_lora_tx_set_explicit_header(&header, &device));

  sx127x_rx_set_callback(lora_rx_callback, &device, &device);
  sx127x_tx_set_callback(lora_tx_callback, NULL, &device);

  gpio_install_isr_service(0);
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << PIN_LORA_DIO),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .intr_type = GPIO_INTR_POSEDGE,
  };
  ESP_ERROR_CHECK(gpio_config(&io_conf));
  ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_LORA_DIO, gpio_isr_handler, NULL));

  xTaskCreate(lora_event_processor_task, "lora_evt", 4096, NULL, 10,
              &lora_event_processor_task_handle);
  xTaskCreate(tx_task, "lora_tx", 4096, &device, 5, &tx_task_handle);
  xTaskCreate(rx_task, "lora_rx", 4096, NULL, 5, NULL);
  xTaskCreate(uart_queuer_task, "uart_q", 4096, NULL, 5, NULL);

  ESP_ERROR_CHECK(
      sx127x_set_opmod(SX127X_MODE_RX_CONT, SX127X_MODULATION_LORA, &device));

  log_info("Gateway started");
}