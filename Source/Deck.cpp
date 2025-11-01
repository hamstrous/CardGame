#include "Deck.h"
#include <limits>
#include <algorithm>
using namespace ax;

// Define DECK_SIZE in cpp file to avoid inline static initialization issues
const ax::Vec2 Deck::DECK_OFFSET{30, 30};
const ax::Vec2 Deck::DECK_SIZE = Card::CARD_SIZE + Deck::DECK_OFFSET;

Deck* Deck::create()
{
    Deck* deck = new (std::nothrow) Deck();
    if (deck && deck->init())
    {
        deck->autorelease();
        return deck;
    }
    AX_SAFE_DELETE(deck);
    return nullptr;
}

bool Deck::init()
{
    if (!Node::init())
        return false;

    _objectSize   = DECK_SIZE;
    _isDraggable  = false;
    _holderOffset = DECK_OFFSET;

    _deckDrawNode = DrawNode::create();
    if (!_deckDrawNode)
    {
        AXLOG("Failed to create DrawNode for Deck");
        return false;
    }

    // Draw a gray rectangle with transparency
    // Position corners relative to center (since node position is center)
    Vec2 bottomLeft(-DECK_SIZE.x / 2, -DECK_SIZE.y / 2);
    Vec2 topRight(DECK_SIZE.x / 2, DECK_SIZE.y / 2);

    // Gray color with 70% opacity (0.7 alpha)
    Color4F fillColor(0.7f, 0.7f, 0.7f, 0.9f);

    _deckDrawNode->drawSolidRect(bottomLeft, topRight, fillColor);

    this->addChild(_deckDrawNode);
    setContentSize(DECK_SIZE);
    return true;
}

void Deck::addCard(Card* card) {
    _cards.push_back(card);
    card->stopAllActions();
    auto moveTo   = MoveTo::create(0.1f, this->getPosition());
    auto easeOut  = EaseOut::create(moveTo, 1.0f);
    auto rotateTo = RotateTo::create(0.1f, this->getRotation());
    card->runAction(rotateTo);
    card->runAction(easeOut);
}

void Deck::addCard(std::vector<Card*> cards) {
    for (auto& card : cards) {
        addCard(card);
    }
}

void Deck::removeCard(Card* card) {
    if (!card)
        return;
    auto it = std::find(_cards.begin(), _cards.end(), card);
    if (it != _cards.end())
    {
        _cards.erase(it);
    }
}

void Deck::removeCard(std::vector<Card*> card) {
    for (auto& c : card) {
        removeCard(c);
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_cards.begin(), _cards.end(), g);
    int zOrder = 0;
    int index = 0;
    float radius = 25.0f;
    for (auto& card : _cards) {
        float angle = AX_DEGREES_TO_RADIANS(360 / _cards.size() * index++);
        Vec2 offset = Vec2(cosf(angle), sinf(angle)) * radius;
        card->setLocalZOrder(zOrder++);
        auto moveAway   = MoveBy::create(0.3f, offset);
        auto moveToward = MoveBy::create(0.3f, -offset);
        auto seq = Sequence::create(moveAway, moveToward, nullptr);
        card->runAction(seq);
    }
}

void Deck::deal(int count, std::vector<Rack*>& racks) {
    for (int i = 0; i < count; ++i) {
        for (auto& rack : racks) {
            if (_cards.empty()) {
                return; // No more cards to deal
            }
            Card* card = _cards.back();
            _cards.pop_back();
            rack->addCard(card);
        }
    }
}
