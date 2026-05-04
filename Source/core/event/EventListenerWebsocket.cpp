#include "EventListenerWebSocket.h"

#include "EventWebSocket.h"

const std::string EventListenerWebSocket::LISTENER_ID = "__ax_websocket";


bool EventListenerWebSocket::checkAvailable()
{
    if (onWebSocketMessage == nullptr || onWebSocketOpen == nullptr || onWebSocketError == nullptr || onWebSocketClose == nullptr)
    {
        AXASSERT(false, "Invalid EventListenerWebSocket!");
        return false;
    }

    return true;
}

EventListenerWebSocket* EventListenerWebSocket::create()
{
    auto ret = new EventListenerWebSocket();
    if (ret->init())
    {
        ret->autorelease();
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerWebSocket* EventListenerWebSocket::clone()
{
    auto ret = new EventListenerWebSocket();
    if (ret->init())
    {
        ret->autorelease();
        ret->onWebSocketMessage = onWebSocketMessage;
        ret->onWebSocketOpen    = onWebSocketOpen;
        ret->onWebSocketError   = onWebSocketError;
        ret->onWebSocketClose   = onWebSocketClose;
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerWebSocket::EventListenerWebSocket() : onWebSocketMessage(nullptr), onWebSocketOpen(nullptr), onWebSocketError(nullptr), onWebSocketClose(nullptr) {}

bool EventListenerWebSocket::init()
{
    auto listener = [this](ax::Event* event) {
        auto webSocketEvent = static_cast<EventWebSocket*>(event);
        switch (webSocketEvent->_eventType)
        {
        case EventWebSocket::WebSocketEventType::MESSAGE:
            if (onWebSocketMessage != nullptr)
                onWebSocketMessage(webSocketEvent);
            break;
        case EventWebSocket::WebSocketEventType::OPEN:
            if (onWebSocketOpen != nullptr)
                onWebSocketOpen(webSocketEvent);
            break;
        #undef ERROR
        case EventWebSocket::WebSocketEventType::ERROR:
        #define ERROR 0
            if (onWebSocketError != nullptr)
                onWebSocketError(webSocketEvent);
            break;
        case EventWebSocket::WebSocketEventType::CLOSE:
            if (onWebSocketClose != nullptr)
                onWebSocketClose(webSocketEvent);
            break;
        default:
            break;
        }   
    };

    if (EventListenerCustom::init(EventListenerWebSocket::LISTENER_ID, listener))
    {
        return true;
    }
    return false;
}
