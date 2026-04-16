#include "Zone.h"

#include "utils/helper.h"
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

    _cardListener = EventListenerCard::create();
    _cardListener->onCardReleased = AX_CALLBACK_1(Zone::OnCardMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_cardListener, this);

    scheduleUpdate();

    return true;
}

void Zone::update(float delta) {}

void Zone::OnCardMouseUp(ax::Event* event) {
    EventCard* cardEvent = static_cast<EventCard*>(event);
    ax::Vec2 releasePosition = cardEvent->getReleasePosition();
    // Check if the card belongs to this zone

    if (isWorldPositionInNode(this, releasePosition))  // containPoint(this,mousePos))
    {
        moveCardToThisZone(cardEvent->getCard(), 0.5f);
    }
}

void Zone::moveCard(Card* card, const ax::Vec2& targetPosition, float duration) {
    // Card must be a child of this zone already
    AXASSERT(card->getParent() == this, "Card must be a child of this zone to move it");

    ax::FiniteTimeAction* moveAction   = ax::MoveTo::create(duration, targetPosition);
    ax::FiniteTimeAction* rotateAction = ax::RotateTo::create(duration, 0);
    ax::FiniteTimeAction* scaleAction  = ax::ScaleTo::create(duration, 1.f);
    ax::Spawn* spawnAction             = ax::Spawn::create(moveAction, rotateAction, scaleAction, nullptr);
    card->runAction(spawnAction);
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
    card->setRotation(getWorldRotation(card) - getWorldRotation(this));  // To get the absolute difference in rotation between the card and the zone and rotate it accordingly
    card->setVecScale(getWorldScale(card) / getWorldScale(this));  // To get the absolute difference in scale between the card and the zone and scale it accordingly
    setNewParentWithNoEffect(card, this);

    ax::Vector<Card*> _cardList = castToVectorOfType<Card*>(this->getChildren());

    moveCard(card, ax::Vec2::ZERO, duration);
}

Zone::~Zone() {}
