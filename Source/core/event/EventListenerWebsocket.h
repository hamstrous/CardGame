#pragma once

#include "axmol.h"

class EventWebSocket;
class AX_DLL EventListenerWebSocket : public ax::EventListenerCustom
{
public:
    static EventListenerWebSocket* create(const std::string& cmd);

    /// Overrides
    virtual EventListenerWebSocket* clone() override;
    virtual bool checkAvailable() override;

    std::function<void(EventWebSocket*)> onWebSocketMessage;
    EventListenerWebSocket();
    bool init(const std::string& cmd);
};
