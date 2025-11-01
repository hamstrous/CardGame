#include "Rack.h"
#include <limits>
#include <algorithm>
using namespace ax;

Rack* Rack::create(const std::string& texture)
{
    Rack* table = new (std::nothrow) Rack();
    if (table && table->init(texture))
    {
        table->autorelease();
        return table;
    }
    AX_SAFE_DELETE(table);
    return nullptr;
}

bool Rack::init(const std::string& texture)
{
    if (!Node::init())
        return false;

    _objectSize  = TABLE_SIZE;
    _isDraggable = false;

    _tableSprite = Sprite::create(texture);
    if (!_tableSprite)
    {
        AXLOG("Failed to load table texture: %s", texture.c_str());
        return false;
    }

    _tableSprite->setContentSize(TABLE_SIZE);
    this->addChild(_tableSprite);
    setContentSize(TABLE_SIZE);
    return true;
}

void Rack::addCard(Card* card)
{
    if (!card)
        return;
     setSpacing(_cards.size() + 1);

    if (_cards.empty())
    {
        addCardAt(card, 0);
        return;
    }

    Vec2 cardPos      = card->getPosition();
    int nearestIndex  = 0;
    float minDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < _cards.size(); i++)
    {
        Vec2 existingCardPos = getCardPosition(i, _cards.size() + 1);
        float distance       = cardPos.distance(existingCardPos);

        if (distance < minDistance)
        {
            minDistance  = distance;
            nearestIndex = i;
        }
    }

    Vec2 lastPos        = getCardPosition(_cards.size(), _cards.size() + 1);
    float distanceToEnd = cardPos.distance(lastPos);
    if (distanceToEnd < minDistance)
    {
        nearestIndex = _cards.size();
    }

    addCardAt(card, nearestIndex);
}

void Rack::removeCard(Card* card)
{
    if (!card)
        return;
    auto it = std::find(_cards.begin(), _cards.end(), card);
    if (it != _cards.end())
    {
        _cards.erase(it);
        if (_cards.size() > 0)
            setSpacing(_cards.size());
        for (int i = 0; i < _cards.size(); i++)
        {
            moveCardToPosition(_cards[i], getCardPosition(i, _cards.size()));
        }
    }
}

void Rack::addCardAt(Card* card, int index)
{
    for (int i = 0; i < index; i++)
    {
        moveCardToPosition(_cards[i], getCardPosition(i, _cards.size() + 1));
    }
    for (int i = index; i < _cards.size(); i++)
    {
        _cards[i]->setLocalZOrder(i + 1);
        moveCardToPosition(_cards[i], getCardPosition(i + 1, _cards.size() + 1));
    }
    card->setLocalZOrder(index);
    _cards.insert(_cards.begin() + index, card);
    moveCardToPosition(card, getCardPosition(index, _cards.size()));
}

void Rack::moveCardToPosition(Card* card, const ax::Vec2& position)
{
    card->stopAllActions();
    auto moveTo  = MoveTo::create(0.3f, position);
    auto easeOut = EaseOut::create(moveTo, 2.0f);
    card->runAction(easeOut);
}

void Rack::startDragging()
{
    _isDragging = true;
    _cards.clear();
}

void Rack::addCard(std::vector<Card*>& cards, Vec2& mousePos)
{
    DraggableObject::sortObjectsByPosition(cards);
    if (cards.empty())
        return;
    setSpacing(_cards.size() + cards.size());

    if (_cards.empty())
    {
        addCardAt(cards, 0);
        return;
    }

    int nearestIndex  = 0;
    float minDistance = std::numeric_limits<float>::max();
    int newSize      = _cards.size() + cards.size();
    for (int i = 0; i < _cards.size(); i++)
    {
        Vec2 existingCardPos = getCardPosition(i, newSize);
        float distance       = mousePos.distance(existingCardPos);

        if (distance < minDistance)
        {
            minDistance  = distance;
            nearestIndex = i;
        }
    }

    Vec2 lastPos        = getCardPosition(_cards.size(), newSize);
    float distanceToEnd = mousePos.distance(lastPos);
    if (distanceToEnd < minDistance)
    {
        nearestIndex = _cards.size();
    }

    addCardAt(cards, nearestIndex);
}

void Rack::removeCard(const std::vector<Card*>& cards)
{
    if (cards.empty())
        return;

    for (auto card : cards)
    {
        auto it = std::find(_cards.begin(), _cards.end(), card);
        if (it != _cards.end())
        {
            _cards.erase(it);
        }
    }
    if (_cards.size() > 0)
        setSpacing(_cards.size());
    for (int i = 0; i < _cards.size(); i++)
    {
        moveCardToPosition(_cards[i], getCardPosition(i, _cards.size()));
    }
}

void Rack::addCardAt(std::vector<Card*>& cards, int index)
{
    int newSize = _cards.size() + cards.size();
    for (int i = 0; i < index; i++)
    {
        _cards[i]->setLocalZOrder(i);
        moveCardToPosition(_cards[i], getCardPosition(i, newSize));
    }
    for (int i = index; i < _cards.size(); i++)
    {
        _cards[i]->setLocalZOrder(i + cards.size());
        moveCardToPosition(_cards[i], getCardPosition(i + cards.size(), newSize));
    }
    for (auto card : cards)
    {
        card->setLocalZOrder(index);
        _cards.insert(_cards.begin() + index, card);
        moveCardToPosition(card, getCardPosition(index++, newSize));
    }
}

ax::Vec2 Rack::getCardPosition(int index, int cardCount)
{
    float startX = this->getPosition().x - (cardCount - 1) * _cardSpacing / 2;

    return ax::Vec2(startX + index * _cardSpacing, this->getPosition().y);
}
