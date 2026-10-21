#include "EventZone.h"
#include "EventListenerZone.h"

EventZone::EventZone(Zone* zone, Card* card) : EventCustom(EventListenerZone::LISTENER_ID), _zone(zone), _card(card) {}
