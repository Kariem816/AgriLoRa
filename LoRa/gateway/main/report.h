#ifndef AGRI_LORA_REPORT_H
#define AGRI_LORA_REPORT_H

#include <packet.h>

typedef struct __attribute__((packed)) {
  Packet packet;
  float snr;
  int32_t frequency_error;
  int16_t rssi;
} ReportPacket;

bool log_info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_warn(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_err(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_packet(const ReportPacket *packet);

#endif // AGRI_LORA_REPORT_H
