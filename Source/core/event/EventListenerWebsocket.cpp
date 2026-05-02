#include "EventListenerWebSocket.h"

#include "EventWebSocket.h"

bool EventListenerWebSocket::checkAvailable()
{
    if (onWebSocketMessage == nullptr)
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
        if (onWebSocketMessage != nullptr)
            onWebSocketMessage(webSocketEvent);
    };

    if (EventListenerCustom::init(cmd, listener))
    {
        return true;
    }
    return false;
}
