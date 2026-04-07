#include "Zone.h"
#include "utils/random.hpp"

using Random = lib::random_static;

Zone* Zone::create(ZoneData* property)
{
    Zone* zone = new (std::nothrow) Zone();
    if (zone && zone->init(property))
    {
        zone->autorelease();
        return zone;
    }
    AX_SAFE_DELETE(zone);
    return nullptr;
}

bool Zone::init(ZoneData* property)
{
    return false;
}

void Zone::shuffleCards() {
    Random::shuffle(_cardList.begin(), _cardList.end());
}

void Zone::sendCardToAnotherZone(Zone* targetZone, Card* card) {}

void Zone::sortCards() {}

void Zone::moveCardToThisZone(const Card* card, float duration) {}

Zone::~Zone() {}
