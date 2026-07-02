#ifndef AGRI_PACKET_H
#define AGRI_PACKET_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint8_t version;
    uint8_t from_node;
    uint8_t to_node;
    uint16_t sequence_number;
} PacketHeader;

typedef struct __attribute__((packed)) {
    uint8_t command_id;
    uint8_t payload_length;
    uint8_t payload[16]; // Adjust size as needed
} Command;

typedef struct __attribute__((packed)) {
  PacketHeader header;
  Command command;
  uint8_t checksum;
} Packet;

typedef struct __attribute__((packed)) {
  Packet packet;
  float snr;
  int32_t frequency_error;
  int16_t rssi;
} ReportPacket;

void packet_calculate_checksum(Packet *packet);
bool packet_verify_checksum(Packet *packet);

#endif //AGRI_PACKET_H