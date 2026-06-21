#ifndef PLUGIN_H
#error "A plugin file must be externally provided at compile time."
#endif

#ifndef NODE_ID
#error "A node ID must be externally provided at compile time."
#endif

#include <SPI.h>
#include <LoRa.h>

#include PLUGIN_H
#include "plugin.hpp"
#include "../common/ring.hpp"

#ifndef PACKET_BUFFER_SIZE
#define PACKET_BUFFER_SIZE 64
#endif

#define CS_PIN 5
#define RESET_PIN 14
#define IRQ_PIN 26

NodePlugin *plugin;
RingBuffer<Packet, PACKET_BUFFER_SIZE> txBuffer;
RingBuffer<Packet, PACKET_BUFFER_SIZE> rxBuffer;

void setup()
{
    Serial.begin(115200); // initialize serial
    while (!Serial)
        ;

    plugin = new Plugin();
    plugin->Init(sendWrapper, NODE_ID);
    plugin->setup();

    LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN); // set CS, reset, IRQ pin

    if (!LoRa.begin(433E6))
    {
        Serial.println("Error: initializing LoRa");
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
            plugin->onPacketReceived(&packet);
        }
    }

    plugin->loop();

    while (!txBuffer.isEmpty())
    {
        Packet packet;
        if (txBuffer.pop(packet))
        {
            sendPacket(&packet);
        }
    }
    LoRa.receive();
}

void onReceive(int packetSize)
{
    if (packetSize < sizeof(Packet))
        return; // if there's no packet, return

    Packet packet;
    LoRa.readBytes((char *)&packet, sizeof(packet));
    if (!rxBuffer.push(packet))
    {
        Serial.println("Warning: RxBuffer overflow, dropping packet");
    }
}

void sendWrapper(const Packet *packet)
{
    if (!txBuffer.push(*packet))
    {
        Serial.println("Warning: TxBuffer overflow, dropping packet");
    }
}

void sendPacket(Packet *packet)
{
    LoRa.beginPacket();
    LoRa.write((const uint8_t *)packet, sizeof(Packet));
    LoRa.endPacket();
}