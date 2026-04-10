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
    ax::DrawNode* _rectNode = ax::DrawNode::create();
    _rectNode->drawRect(ax::Vec2::ZERO, ax::Vec2::ZERO, ax::Color4F::WHITE);

    return true;
}

void Zone::shuffleCards() {
    Random::shuffle(_cardList.begin(), _cardList.end());
}

void Zone::sendCardToAnotherZone(Zone* targetZone, Card* card) {
    _cardList.erase(std::remove(_cardList.begin(), _cardList.end(), card), _cardList.end());
    targetZone->moveCardToThisZone(card);
}

void Zone::sortCards() {}

void Zone::moveCardToThisZone(Card* card, float duration) {
    _cardList.pushBack(card);
    card->setParent(this);
}

Zone::~Zone() {}
