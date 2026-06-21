#include <SPI.h>
#include <LoRa.h>

#include "reported.h"
#include "../common/ring.hpp"

#define CS_PIN 5     // SX1278 NSS; LoRa radio chip select
#define RESET_PIN 14 // SX1278 RDT; LoRa radio reset
#define IRQ_PIN 26   // SX1278 DIO; Interrupt Request Pin; must be a hardware interrupt pin

#ifndef RECEIVE_BUFFER_SIZE
#define RECEIVE_BUFFER_SIZE 256
#endif

RingBuffer<Packet, RECEIVE_BUFFER_SIZE> rxBuffer;

void setup()
{
    Serial.begin(115200); // initialize serial
    while (!Serial)
        ;

    // override the default CS, reset, and IRQ pins (optional)
    LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN); // set CS, reset, IRQ pin

    if (!LoRa.begin(433E6))
    {
        reportError();
        while (true)
            ;
    }

    LoRa.onReceive(onReceive);
    LoRa.receive();
}

void loop()
{
    while (!rxBuffer.isEmpty())
    {
        Packet packet;
        if (rxBuffer.pop(packet))
        {
            reportPacket(&packet);
        }
    }

    if (Serial.available())
    {
        while (Serial.available() >= sizeof(Packet))
        {
            Packet message;
            Serial.readBytes((char *)&message, sizeof(Packet));
            sendPacket(&message);
        }
        LoRa.receive(); // go back to receive mode
    }
}

void onReceive(int packetSize)
{
    if (packetSize < sizeof(Packet))
        return; // if packet is not complete, ignore it

    Packet packet;
    LoRa.readBytes((char *)&packet, sizeof(Packet));
    if (!rxBuffer.push(packet))
    {
        // Somehow report overflow
    }
}

void sendPacket(Packet *packet)
{
    LoRa.beginPacket();
    LoRa.write((const uint8_t *)packet, sizeof(Packet));
    LoRa.endPacket();
}

void reportPacket(Packet *packet)
{
    ReportedMessage report;
    report.rssi = LoRa.packetRssi();
    report.snr = LoRa.packetSnr();
    report.frequency_error = LoRa.packetFrequencyError();
    report.packet = *packet;
    Serial.write((const char *)&report, sizeof(ReportedMessage));
}

void reportError()
{
    ReportedMessage error = {0};
    Serial.write((const char *)&error, sizeof(ReportedMessage));
}