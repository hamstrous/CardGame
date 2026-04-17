#include "EventListenerZone.h"


const std::string EventListenerZone::LISTENER_ID = "__ax_card";

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
        ret->onCardFlipped  = onCardFlipped;
        ret->onCardReleased = onCardReleased;
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerZone::EventListenerZone() : onCardFlipped(nullptr), onCardReleased(nullptr) {}

bool EventListenerZone::init()
{
    auto listener = [this](ax::Event* event) {
        auto cardEvent = static_cast<EventCard*>(event);
        if (cardEvent->_isCardFlipped)
        {
            if (onCardFlipped != nullptr)
                onCardFlipped(cardEvent);
        }
        else
        {
            if (onCardReleased != nullptr)
                onCardReleased(cardEvent);
        }
    };

    if (EventListenerCustom::init(LISTENER_ID, listener))
    {
        return true;
    }

    return false;
}
