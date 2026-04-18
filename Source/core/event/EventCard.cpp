#include "EventCard.h"
#include "EventListenerCard.h"

EventCard::EventCard(Card* card, bool isFlipped)
    : EventCustom(EventListenerCard::LISTENER_ID)
    , card(card)
    , _isCardFlipped(isFlipped)
{}

EventCard::EventCard(Card* card, ax::Vec2 releasePosition)
    : EventCustom(EventListenerCard::LISTENER_ID)
    , card(card)
    , _releasePosition(releasePosition)
    , _isCardFlipped(false) 
{}
