#ifndef AGRI_GATEWAY_FRAME_H
#define AGRI_GATEWAY_FRAME_H

#include <stdint.h>
#include <stdarg.h>

typedef enum {
  FRAME_INFO,
  FRAME_WARN,
  FRAME_ERROR,
  FRAME_PKT,
} FrameType;

typedef struct {
  uint8_t magic;
  uint8_t type;
  uint16_t len;
} __attribute__((packed)) FrameHeader;

#define FRAME_MAGIC 0xCA
#define FRAME_MAX_PAYLOAD 256

typedef struct {
  FrameType type;
  uint16_t len;
  uint8_t payload[FRAME_MAX_PAYLOAD];
} Frame;

// All messages must be at most 256 bytes long. Longer messages will be truncated.
Frame frame_vlog(FrameType type, const char *fmt, va_list args);
Frame frame_info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
Frame frame_warn(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
Frame frame_error(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif //AGRI_GATEWAY_FRAME_H