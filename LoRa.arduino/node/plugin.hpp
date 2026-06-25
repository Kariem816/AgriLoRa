#ifndef AGRI_LORA_NODE_PLUGIN_HPP
#define AGRI_LORA_NODE_PLUGIN_HPP

#include <cstdint>
#include <functional>

#include "../common/packet.h"

using SendFunction = std::function<void(const Packet *)>;

class NodePlugin
{
public:
    virtual ~NodePlugin() = default;
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void onPacketReceived(const Packet *message) = 0;
    virtual char *Name() = 0;

    void Init(SendFunction sendFunction, uint8_t nodeId)
    {
        sendFn = sendFunction;
        this->nodeId = nodeId;
    }

protected:
    void send(const Packet *message)
    {
        sendFn(message);
    }

    uint8_t NodeId() const
    {
        return nodeId;
    }

private:
    SendFunction sendFn;
    uint8_t nodeId;
};

#endif // AGRI_LORA_NODE_PLUGIN_HPP
