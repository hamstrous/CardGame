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

EventListenerWebSocket* EventListenerWebSocket::create(const std::string& cmd)
{
    auto ret = new EventListenerWebSocket();
    if (ret->init(cmd))
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
    if (ret->init(_listenerID))
    {
        ret->autorelease();
        ret->onWebSocketMessage = onWebSocketMessage;
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerWebSocket::EventListenerWebSocket() : onWebSocketMessage(nullptr) {}

bool EventListenerWebSocket::init(const std::string& cmd)
{
    auto listener = [this](ax::Event* event) {
        auto webSocketEvent = static_cast<EventWebSocket*>(event);
        switch (webSocketEvent->_eventType)
        {
        case EventWebSocket::WebSocketEventType::MESSAGE:
            if (onWebSocketMessage != nullptr)
                if (_messageCmd.empty() || _messageCmd == webSocketEvent->_cmd)
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

    if (EventListenerCustom::init(cmd, listener))
    {
        return true;
    }
    return false;
}
