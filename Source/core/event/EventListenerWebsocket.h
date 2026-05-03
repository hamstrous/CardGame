#pragma once

#include "axmol.h"

class EventWebSocket;
class AX_DLL EventListenerWebSocket : public ax::EventListenerCustom
{
public:
    static const std::string LISTENER_ID;

    static EventListenerWebSocket* create(const std::string& cmd);

    /// Overrides
    virtual EventListenerWebSocket* clone() override;
    virtual bool checkAvailable() override;

    std::function<void(EventWebSocket*)> onWebSocketMessage;
    std::function<void(EventWebSocket*)> onWebSocketOpen;
    std::function<void(EventWebSocket*)> onWebSocketError;
    std::function<void(EventWebSocket*)> onWebSocketClose;

    EventListenerWebSocket();
    bool init(const std::string& cmd);

private:
    std::string _messageCmd;
};
