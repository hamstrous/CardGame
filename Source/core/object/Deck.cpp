#include "Deck.h"

#include "utils/helper.h"
#include "utils/random.hpp"

#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include <algorithm>

using Random = lib::random_static;

Deck* Deck::create(ZoneData* property)
{
    Deck* zone = new (std::nothrow) Deck();
    if (zone && zone->init(property))
    {
        zone->autorelease();
        return zone;
    }
    AX_SAFE_DELETE(zone);
    return nullptr;
}

bool Deck::init(ZoneData* property)
{
    this->setAnchorPoint(ax::Vec2(0.5f, 0.5f));

    _rectNode = ax::DrawNode::create();
    this->addChild(_rectNode);
    _rectNode->drawRect(ax::Vec2::ZERO, ax::Vec2::ZERO, ax::Color4F::WHITE);

    _cardListener = EventListenerCard::create();
    _cardListener->onCardReleased = AX_CALLBACK_1(Zone::OnCardMouseUp, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_cardListener, this);

    _mouseListener = ax::EventListenerMouse::create();

    scheduleUpdate();

    this->setRotationSkewX(80);
    return true;
}

void Deck::update(float delta) {}

void Deck::shuffleCards() {
    std::vector<ax::ActionInterval*> cardsMoveToCenterActions;
    auto cardList = helper::castToVectorOfType<Card*>(this->getChildren());

    std::vector<ax::Vector<ax::ActionInterval*>> actionListForEachCard( cardList.size() );

    for (int i = 0; i < cardList.size(); i++)
    {
        // move to center
        auto card                              = cardList[i];
        ax::Vec2 center                        = getContentSize() / 2;
        ax::ActionInterval* moveToCenterAction = ax::MoveTo::create(1.f, center);
        actionListForEachCard[i].pushBack(moveToCenterAction);

        // move left or right
        bool moveRight = i % 2 == 0;
        float spacing  = 20.f;
        auto moveBy = ax::MoveBy::create(1.f, ax::Vec2((moveRight ? 1 : -1) * spacing, 0));
        actionListForEachCard[i].pushBack(moveBy);

        // move up
        auto moveUp = ax::MoveBy::create(1.f, ax::Vec2(0, i/2 * 20));
        actionListForEachCard[i].pushBack(moveUp);

        // move back to center collumn
        auto moveBackToCenter = ax::MoveBy::create(1.f, ax::Vec2(0, -i/2 * 20));
        actionListForEachCard[i].pushBack(moveBackToCenter);
    }


    // Create sequence for each card
    for (int i = 0; i < cardList.size(); i++)
    {
        auto card       = cardList[i];
        auto actionList = actionListForEachCard[i];
        auto sequence = ax::Sequence::create(helper::castToVectorOfType<ax::FiniteTimeAction*>(actionList));
        card->runAction(sequence);
    }
}

void Deck::lockInput() {}

void Deck::unlockInput() {}

Deck::~Deck() {}
