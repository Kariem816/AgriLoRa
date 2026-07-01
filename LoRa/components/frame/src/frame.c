#include "frame.h"

#include <stdio.h>
#include <stdarg.h>

Frame frame_vlog(FrameType type, const char *fmt, va_list args) { 
  Frame frame = {0}; 
  frame.type = type;
  frame.len = vsnprintf((char *)frame.payload, FRAME_MAX_PAYLOAD, fmt, args);
  return frame;
}

Frame frame_info(const char *fmt, ...) {
  Frame frame = {0};
  frame.type = FRAME_INFO;
  va_list args;
  va_start(args, fmt);
  frame.len = vsnprintf((char *)frame.payload, FRAME_MAX_PAYLOAD, fmt, args);
  va_end(args);
  return frame;
}

Frame frame_warn(const char *fmt, ...) {
  Frame frame = {0};
  frame.type = FRAME_WARN;
  va_list args;
  va_start(args, fmt);
  frame.len = vsnprintf((char *)frame.payload, FRAME_MAX_PAYLOAD, fmt, args);
  va_end(args);
  return frame;
}

Frame frame_error(const char *fmt, ...) {
  Frame frame = {0};
  frame.type = FRAME_ERROR;
  va_list args;
  va_start(args, fmt);
  frame.len = vsnprintf((char *)frame.payload, FRAME_MAX_PAYLOAD, fmt, args);
  va_end(args);
  return frame;
}
