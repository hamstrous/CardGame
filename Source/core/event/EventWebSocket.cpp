#include "EventWebSocket.h"
#include "EventListenerWebSocket.h"

EventWebSocket::EventWebSocket(const WebSocketEventType eventType, const std::string& cmd, const json& data)
    : EventCustom(EventListenerWebSocket::LISTENER_ID) 
    , _eventType(eventType)
    , _cmd(cmd)
    , _data(data)
{}

