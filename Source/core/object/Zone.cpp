#include "Zone.h"

#include "utils/helper.h"
#include "utils/random.hpp"

#include "core/event/EventCard.h"

#include <algorithm>

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

// Positions will be spread on the horizontal line, without leaving the zone
std::vector<ax::Vec2> Zone::getCurrentPositionList(ax::Vector<Card*> cardList)
{
    if (cardList.empty())
    {
        cardList = castToVectorOfType<Card*>(this->getChildren());
    }
    int size              = cardList.size();
    ax::Vec2 origin       = getAnchorPoint() * getContentSize();
    ax::Vec2 originOffset = ax::Vec2::ZERO;
    for (auto card : cardList)
    {
        AXASSERT(card->getParent() == this, "Card list should only contain cards that are children of this zone");
        originOffset += ax::Vec2(card->getContentSize().width/ 2, 0);  // Assuming the anchor point of the card is at its center
    }
    std::vector<ax::Vec2> positions;
    if (size > 0)
    {
        ax::Vec2 previousCardEnd = ax::Vec2(std::max((origin - originOffset).x, origin.x - this->getContentSize().width / 2), origin.y);
        float overflow           = originOffset.x * 2 > this->getContentSize().width ? originOffset.x * 2 - this->getContentSize().width : 0;
        for (int i = 0; i < size; ++i)
        {
            float spacing = (i == 0) ? 0 : overflow * (cardList.at(i)->getContentSize().width / (originOffset.x * 2 - cardList.at(0)->getContentSize().width));
            positions.push_back(previousCardEnd + ax::Vec2(cardList.at(i)->getContentSize().width / 2, 0) - ax::Vec2(spacing, 0));
            previousCardEnd += ax::Vec2(cardList.at(i)->getContentSize().width - spacing, 0);
        }
    }

    return positions;
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
    std::vector<ax::Vec2> newPositions = getCurrentPositionList();
    for (int i = 0; i < _cardList.size(); i++)
    {
        AXLOG("Positioning card %d at position (%f, %f)", i, newPositions.at(i).x, newPositions.at(i).y);
        moveCard(_cardList.at(i), newPositions.at(i), duration);
    }
}

Zone::~Zone() {}
