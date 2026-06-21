#include "../plugin.hpp"

class Plugin : public NodePlugin
{
public:
    void setup() override
    {
        Serial.print("Plugin (");
        Serial.print(Name());
        Serial.println(") setup");
    }

    void loop() override
    {
        // Example: send a packet every 5 seconds
        static unsigned long lastSendTime = 0;
        if (millis() - lastSendTime > 5000)
        {
            Packet packet = {0};
            packet.header.version = 1;
            packet.header.from_node = NodeId(); // Example node ID
            packet.header.to_node = 0;          // Broadcast
            packet.header.sequence_number = ++seq;
            packet.command.command_id = 0; // Example command ID
            packet.command.payload_length = 0;

            Serial.print("Plugin (");
            Serial.print(Name());
            Serial.println(") sending a packet:");
            Serial.print("  From Node: ");
            Serial.println(packet.header.from_node);
            Serial.print("  To Node: ");
            Serial.println(packet.header.to_node);
            Serial.print("  Sequence Number: ");
            Serial.println(packet.header.sequence_number);
            Serial.print("  Command ID: ");
            Serial.println(packet.command.command_id);
            Serial.print("  Payload Length: ");
            Serial.println(packet.command.payload_length);
            Serial.print("  Checksum: ");
            Serial.println(packet.checksum, HEX);
            Serial.println();
            
            send(&packet);
            lastSendTime = millis();
        }
    }

    void onPacketReceived(const Packet *message) override
    {
        Serial.print("Plugin (");
        Serial.print(Name());
        Serial.println(") received a packet:");
        Serial.print("  From Node: ");
        Serial.println(message->header.from_node);
        Serial.print("  To Node: ");
        Serial.println(message->header.to_node);
        Serial.print("  Sequence Number: ");
        Serial.println(message->header.sequence_number);
        Serial.print("  Command ID: ");
        Serial.println(message->command.command_id);
        Serial.print("  Payload Length: ");
        Serial.println(message->command.payload_length);
    }

    char *Name() override
    {
        return "DefaultPlugin";
    }

private:
    uint64_t seq = 0;
};