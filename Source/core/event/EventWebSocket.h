#pragma once

#include "axmol.h"

#include <string>
#include "utils/json.hpp"

using json = lib::json;

class AX_DLL EventWebSocket : public ax::EventCustom
{
public:
    enum class WebSocketEventType
    {
        MESSAGE,
        OPEN,
        CLOSE,
#undef ERROR
        ERROR
#define ERROR 0
    };

    EventWebSocket(const WebSocketEventType eventType, const json& data = json());

    json getData() const { return _data; }
    WebSocketEventType getEventType() const { return _eventType; }

private:
    json _data;
    WebSocketEventType _eventType;
    
    friend class EventListenerWebSocket;
};


