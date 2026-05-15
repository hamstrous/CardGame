#include "Deck.h"

#include "utils/helper.h"
#include "utils/random.hpp"

#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include <algorithm>

using namespace helper;
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

    //this->setRotationSkewX(80);

    _rectNode->setColor(ax::Color3B::RED);
    return true;
}

void Deck::update(float delta) {}

void Deck::shuffleCards() {
    std::vector<ax::ActionInterval*> cardsMoveToCenterActions;
    auto cardList = helper::castToVectorOfType<Card*>(this->getChildren());

    std::vector<int> originalIndexList;
    for (int i = 0; i < cardList.size(); i++)
        originalIndexList.push_back(i);

    int cardCount = (int)cardList.size();
    if (cardCount == 1)
    {
        AXLOGD("Only one card in the deck, no need to shuffle");
        return;
    }
    std::vector<ax::Vector<ax::FiniteTimeAction*>> actionListForEachCard( cardCount );

    // select random amount of consecutive card to move right
    float spacing = 10.f;

    for (int i = 0; i < cardList.size(); i++)
    {
        // move to center
        auto card                              = cardList[i];
        ax::Vec2 center                        = getContentSize() / 2;
        ax::ActionInterval* moveToCenterAction = ax::MoveTo::create(0.5f, center);
        actionListForEachCard[i].pushBack(moveToCenterAction);
        // move up
        auto moveBy   = ax::MoveBy::create(0.1f, ax::Vec2(0, spacing * i));
        actionListForEachCard[i].pushBack(moveBy);
    }

    for (int turn = 0; turn < 20; turn++)
    {
        int start = Random::get(0, (int)cardList.size() - 2);
        int end   = Random::get(start, (int)cardList.size() - 2);

        for (int index = 0; index < cardList.size(); index++)
        {
            int i = originalIndexList[index];

            // card from start to end will move right then move up top of the deck
            // for (int j = start; j <= end; j++)
            if (index >= start && index <= end)
            {
                auto moveRight = ax::MoveBy::create(0.1f, ax::Vec2(40.f, 0));
                actionListForEachCard[i].pushBack(moveRight);

                // bring to front before move right to prevent being covered by other cards
                if (index == start)
                {
                    ax::Vector<Card*> cardsToMoveToFront;
                    for (int k = start; k <= end; k++)
                    {
                        int originalIndex = originalIndexList[k];
                        cardsToMoveToFront.pushBack(cardList[originalIndex]);
                    }

                    auto bringToFront =
                        ax::CallFunc::create([this, cardsToMoveToFront]() { moveCardListToTop(cardsToMoveToFront); });
                    actionListForEachCard[i].pushBack(dynamic_cast<ax::FiniteTimeAction*>(bringToFront));
                }

                auto moveUpLeft = ax::MoveBy::create(0.1f, ax::Vec2(-40.f, spacing * ((cardCount - 1) - end)));
                actionListForEachCard[i].pushBack(moveUpLeft);
            }

            // card from 0 to start - 1 will stand still
            // for (int j = 0; j < start; j++)
            if (index < start)
            {
                auto moveNone = ax::MoveBy::create(0.2f, ax::Vec2::ZERO);
                actionListForEachCard[i].pushBack(moveNone);
            }

            // card from end + 1 to last will move down to fill the gap
            // for (int j = end + 1; j < cardList.size(); j++)
            if (index > end)
            {
                auto moveDown = ax::MoveBy::create(0.2f, ax::Vec2(0, -spacing * (end - start + 1)));
                actionListForEachCard[i].pushBack(moveDown);
            }

            // move back to center collumn
            // auto moveBackToCenter = ax::MoveTo::create(1.f, getContentSize() / 2);
            // actionListForEachCard[i].pushBack(moveBackToCenter);
        }
        // remove index from start to end and insert to the back
        for (int c = start; c <= end; c++)
        {
            int index = originalIndexList[c];
            originalIndexList.push_back(index);
        }
        originalIndexList.erase(originalIndexList.begin() + start, originalIndexList.begin() + end + 1);
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

void Deck::shuffleCardsWithSeed(std::string seed) {
    auto shuffleSteps = helper::split(seed, ' ');

    std::vector<ax::ActionInterval*> cardsMoveToCenterActions;
    auto cardList = helper::castToVectorOfType<Card*>(this->getChildren());

    std::vector<int> originalIndexList;
    for (int i = 0; i < cardList.size(); i++)
        originalIndexList.push_back(i);

    int cardCount = (int)cardList.size();
    if (cardCount == 1)
    {
        AXLOGD("Only one card in the deck, no need to shuffle");
        return;
    }
    std::vector<ax::Vector<ax::FiniteTimeAction*>> actionListForEachCard(cardCount);

    // select random amount of consecutive card to move right
    float spacing = 10.f;

    for (int i = 0; i < cardList.size(); i++)
    {
        // move to center
        auto card                              = cardList[i];
        ax::Vec2 center                        = getContentSize() / 2;
        ax::ActionInterval* moveToCenterAction = ax::MoveTo::create(0.5f, center);
        actionListForEachCard[i].pushBack(moveToCenterAction);
        // move up
        auto moveBy = ax::MoveBy::create(0.1f, ax::Vec2(0, spacing * i));
        actionListForEachCard[i].pushBack(moveBy);
    }

    for (int seedIndex = 0; seedIndex < shuffleSteps.size(); seedIndex+=2)
    {
        int start = std::stoi(shuffleSteps[seedIndex]);
        int end   = std::stoi(shuffleSteps[seedIndex + 1]);

        for (int index = 0; index < cardList.size(); index++)
        {
            int i = originalIndexList[index];

            // card from start to end will move right then move up top of the deck
            // for (int j = start; j <= end; j++)
            if (index >= start && index <= end)
            {
                auto moveRight = ax::MoveBy::create(0.1f, ax::Vec2(40.f, 0));
                actionListForEachCard[i].pushBack(moveRight);

                // bring to front before move right to prevent being covered by other cards
                if (index == start)
                {
                    ax::Vector<Card*> cardsToMoveToFront;
                    for (int k = start; k <= end; k++)
                    {
                        int originalIndex = originalIndexList[k];
                        cardsToMoveToFront.pushBack(cardList[originalIndex]);
                    }

                    auto bringToFront =
                        ax::CallFunc::create([this, cardsToMoveToFront]() { moveCardListToTop(cardsToMoveToFront); });
                    actionListForEachCard[i].pushBack(dynamic_cast<ax::FiniteTimeAction*>(bringToFront));
                }

                auto moveUpLeft = ax::MoveBy::create(0.1f, ax::Vec2(-40.f, spacing * ((cardCount - 1) - end)));
                actionListForEachCard[i].pushBack(moveUpLeft);
            }

            // card from 0 to start - 1 will stand still
            // for (int j = 0; j < start; j++)
            if (index < start)
            {
                auto moveNone = ax::MoveBy::create(0.2f, ax::Vec2::ZERO);
                actionListForEachCard[i].pushBack(moveNone);
            }

            // card from end + 1 to last will move down to fill the gap
            // for (int j = end + 1; j < cardList.size(); j++)
            if (index > end)
            {
                auto moveDown = ax::MoveBy::create(0.2f, ax::Vec2(0, -spacing * (end - start + 1)));
                actionListForEachCard[i].pushBack(moveDown);
            }

            // move back to center collumn
            // auto moveBackToCenter = ax::MoveTo::create(1.f, getContentSize() / 2);
            // actionListForEachCard[i].pushBack(moveBackToCenter);
        }
        // remove index from start to end and insert to the back
        for (int c = start; c <= end; c++)
        {
            int index = originalIndexList[c];
            originalIndexList.push_back(index);
        }
        originalIndexList.erase(originalIndexList.begin() + start, originalIndexList.begin() + end + 1);
    }

    // Create sequence for each card
    for (int i = 0; i < cardList.size(); i++)
    {
        auto card       = cardList[i];
        auto actionList = actionListForEachCard[i];
        auto sequence   = ax::Sequence::create(helper::castToVectorOfType<ax::FiniteTimeAction*>(actionList));
        card->runAction(sequence);
    }
}

std::string Deck::getRandomShuffleSeed()
{
    std::vector<ax::ActionInterval*> cardsMoveToCenterActions;
    auto cardList = helper::castToVectorOfType<Card*>(this->getChildren());
    std::string seed;
    for (int turn = 0; turn < 20; turn++)
    {
        int start = Random::get(0, (int)cardList.size() - 2);
        int end   = Random::get(start, (int)cardList.size() - 2);
        seed += std::to_string(start) + " " + std::to_string(end) + " ";
    }
    return seed;
}

void Deck::dealCards(ax::Vector<Zone*>& targetZones, int amountPerZone) {
    ax::Vector<ax::FiniteTimeAction*> actionList;
    float delay = 0.6f;
    auto cardList = helper::castToVectorOfType<Card*>(this->getChildren());
    int cardIndex = cardList.size() - 1;

    for (int c = 0; c < amountPerZone; c++)
    {
        for(int i = 0; i < targetZones.size(); i++)
        {
            actionList.pushBack(ax::DelayTime::create(delay));
            auto moveCardAction = ax::CallFunc::create([this, cardList, cardIndex, targetZones, i]() {
                targetZones.at(i)->moveCardToThisZone(cardList.at(cardIndex), 0.5f);
            });
            cardIndex--;
            actionList.pushBack(moveCardAction);
            if (cardIndex >= cardList.size() || cardIndex < 0){
                AXLOGD("Not enough cards in the deck to deal, stop at card index: {}", cardIndex);
                auto sequence = ax::Sequence::create(actionList);
                this->runAction(sequence);
                break;
            }
        }
    }
    auto sequence = ax::Sequence::create(actionList);
    this->runAction(sequence);
}

void Deck::moveCardListToTop(ax::Vector<Card*> cardList) {
    for (auto card : cardList)
    {
        moveNodeToFront(card);
    }
}

std::vector<ax::Vec2> Deck::getCurrentPositionList(ax::Vector<Card*> cardList)
{
    if (cardList.empty())
    {
        cardList = castToVectorOfType<Card*>(this->getChildren());
    }
    int size              = cardList.size();
    ax::Vec2 origin       = getAnchorPoint() * getContentSize();
    ax::Vec2 offset       = ax ::Vec2(0, 5);
    for (auto card : cardList)
    {
        AXASSERT(card->getParent() == this, "Card list should only contain cards that are children of this zone");
    }
    std::vector<ax::Vec2> positions;
    if (size > 0)
    {
        ax::Vec2 spacing = ax::Vec2::ZERO;
        for (int i = 0; i < size; ++i)
        {
            positions.push_back(origin + spacing);
            spacing += offset;
        }
    }

    return positions;
}

void Deck::lockInput() {}

void Deck::unlockInput() {}

Deck::~Deck() {}
