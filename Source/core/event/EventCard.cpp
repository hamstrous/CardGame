
#include "EventCard.h"
#include "EventListenerCard.h"

EventCard::EventCard(Card* card, bool isFlipped) : EventCustom(EventListenerCard::LISTENER_ID), card(card), _isCardFlipped(isFlipped) {}
