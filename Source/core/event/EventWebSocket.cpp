#include "EventWebSocket.h"
#include "EventListenerWebSocket.h"

EventWebSocket::EventWebSocket(const WebSocketEventType eventType, const json& data)
    : EventCustom(EventListenerWebSocket::LISTENER_ID) 
    , _eventType(eventType)
    , _data(data)
{}

