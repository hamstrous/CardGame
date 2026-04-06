#include "Zone.h"

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

void Zone::shuffle() {}

void Zone::sendCardTo(Zone* targetZone, Card* card) {}

void Zone::sortCards() {}
