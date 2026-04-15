#include "Zone.h"
#include "utils/random.hpp"
#include "core/event/EventCard.h"

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
    this->setAnchorPoint(ax::Vec2(0.5f, 0.5f));

    _rectNode = ax::DrawNode::create();
    this->addChild(_rectNode);
    _rectNode->drawRect(ax::Vec2::ZERO, ax::Vec2::ZERO, ax::Color4F::WHITE);

    scheduleUpdate();

    return true;
}

void Zone::update(float delta) {}

void Zone::OnCardMouseUp(ax::Event* event) {
    EventCard* cardEvent = static_cast<EventCard*>(event);
    ax::Vec2 releasePosition = cardEvent->getReleasePosition();
    // Check if the card belongs to this zone

    if (this->getBoundingBox().containsPoint(releasePosition))  // containPoint(this,mousePos))
    {
        moveCardToThisZone(cardEvent->getCard(), 0.5f);
    }
}

void Zone::shuffleCards()
{
    Random::shuffle(_cardList.begin(), _cardList.end());
}

void Zone::sendCardToAnotherZone(Zone* targetZone, Card* card) {
    _cardList.erase(std::remove(_cardList.begin(), _cardList.end(), card), _cardList.end());
    targetZone->moveCardToThisZone(card);
}

void Zone::sortCards() {}

void Zone::setContentSize(const ax::Size& contentSize)
{
    Node::setContentSize(contentSize);
    _rectNode->clear();
    _rectNode->drawRect(ax::Vec2::ZERO, contentSize, ax::Color4F::WHITE);
}

void Zone::moveCardToThisZone(Card* card, float duration) {
    //_cardList.pushBack(card);
    card->retain();
    card->removeFromParent();
    this->addChild(card);
    card->release();

    ax::Action* moveAction = ax::MoveTo::create(duration, ax::Vec2::ZERO);
    card->runAction(moveAction);
}

Zone::~Zone() {}
