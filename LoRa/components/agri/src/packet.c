#include "packet.h"

void packet_calculate_checksum(Packet *packet) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < sizeof(*packet) - 1; i++) {
    checksum ^= ((uint8_t *)packet)[i];
  }
  packet->checksum = checksum;
}   

bool packet_verify_checksum(Packet *packet) {
  uint8_t checksum = 0;
  for (size_t i = 0; i < sizeof(*packet) - 1; i++) {
    checksum ^= ((uint8_t *)packet)[i];
  }
  return checksum == packet->checksum;
}