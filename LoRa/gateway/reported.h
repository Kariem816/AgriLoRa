#ifndef AGRI_LORA_REPORTED_H
#define AGRI_LORA_REPORTED_H

#include "../common/packet.h"

typedef struct __attribute__((packed)) {
    int rssi; // signed 32-bit in esp32
    float snr;
    long frequency_error; // somehow also signed 32-bit in esp32, in Hz
    Packet packet;
} ReportedMessage;

#endif //AGRI_LORA_REPORTED_H