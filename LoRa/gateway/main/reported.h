#ifndef AGRI_LORA_REPORTED_H
#define AGRI_LORA_REPORTED_H

#include <packet.h>

typedef struct __attribute__((packed)) {
    Packet packet;
    float snr;
    int32_t frequency_error;
    int16_t rssi;
} ReportedMessage;

#endif //AGRI_LORA_REPORTED_H