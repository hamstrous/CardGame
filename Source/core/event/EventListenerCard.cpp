#include "EventListenerCard.h"


const std::string EventListenerCard::LISTENER_ID = "__ax_card";

bool EventListenerCard::checkAvailable()
{
    if (onCardFlipped == nullptr && onCardReleased == nullptr)
    {
        AXASSERT(false, "Invalid EventListenerCard!");
        return false;
    }

    return true;
}

EventListenerCard* EventListenerCard::create()
{
    auto ret = new EventListenerCard();
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

EventListenerCard* EventListenerCard::clone()
{
    auto ret = new EventListenerCard();
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

EventListenerCard::EventListenerCard() : onCardFlipped(nullptr), onCardReleased(nullptr) {}

bool EventListenerCard::init()
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
