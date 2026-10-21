#include "EventCard.h"
#include "EventListenerCard.h"

EventCard::EventCard(Card* card, bool isFlipped, bool isClicked)
    : EventCustom(EventListenerCard::LISTENER_ID)
    , card(card)
    , _isCardFlipped(isFlipped)
    , _isCardClicked(isClicked)
{}

EventCard::EventCard(Card* card, ax::Vec2 releasePosition)
    : EventCustom(EventListenerCard::LISTENER_ID)
    , card(card)
    , _releasePosition(releasePosition)
    , _isCardFlipped(false) 
    , _isCardClicked(false) 
{}
