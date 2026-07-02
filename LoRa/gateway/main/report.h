#ifndef AGRI_LORA_REPORT_H
#define AGRI_LORA_REPORT_H

#include <packet.h>

bool log_dbg(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_warn(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_err(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
bool log_packet(const ReportPacket *packet);

#endif // AGRI_LORA_REPORT_H
