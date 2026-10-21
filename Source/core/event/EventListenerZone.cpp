#include "EventListenerZone.h"
#include "EventZone.h"


const std::string EventListenerZone::LISTENER_ID = "__ax_zone";

bool EventListenerZone::checkAvailable()
{
    if (onCardReceived == nullptr)
    {
        AXASSERT(false, "Invalid EventListenerZone!");
        return false;
    }

    return true;
}

EventListenerZone* EventListenerZone::create()
{
    auto ret = new EventListenerZone();
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

EventListenerZone* EventListenerZone::clone()
{
    auto ret = new EventListenerZone();
    if (ret->init())
    {
        ret->autorelease();
        ret->onCardReceived = onCardReceived;
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerZone::EventListenerZone() : onCardReceived(nullptr){}

bool EventListenerZone::init()
{
    auto listener = [this](ax::Event* event) {
        auto zoneEvent = static_cast<EventZone*>(event);
        if (zoneEvent->_isReceived)
        {
            if (onCardReceived != nullptr)
                onCardReceived(zoneEvent);
        }
    };

    if (EventListenerCustom::init(LISTENER_ID, listener))
    {
        return true;
    }

    return false;
}
