#include "Holder.h"
#include <limits>
#include <algorithm>
using namespace ax;

void Holder::addCard(Card* card)
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

void Holder::removeCard(Card* card)
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

void Holder::addCardAt(Card* card, int index)
{
    for (int i = 0; i < index; i++)
    {
        _cards[i]->setLocalZOrder(i);
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

void Holder::moveCardToPosition(Card* card, const ax::Vec2& position)
{
    card->stopAllActionsByTag(Card::MOVE_TO_HOLDER_TAG);
    auto moveTo   = MoveTo::create(0.3f, position);
    auto easeOut  = EaseOut::create(moveTo, 2.0f);
    auto rotateTo = RotateTo::create(0.3f, this->getRotation());
    easeOut->setTag(Card::MOVE_TO_HOLDER_TAG);
    rotateTo->setTag(Card::MOVE_TO_HOLDER_TAG);
    card->runAction(rotateTo);
    card->runAction(easeOut);
}

void Holder::startDragging()
{
    _isDragging = true;
    _cards.clear();
}

void Holder::addCard(std::vector<Card*>& cards, Vec2& mousePos)
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
    int newSize       = _cards.size() + cards.size();
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

void Holder::removeCard(const std::vector<Card*>& cards)
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

void Holder::addCardAt(std::vector<Card*>& cards, int index)
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

void Holder::setSpacing(int cardCount)
{
    if (cardCount <= 1)
    {
        _cardSpacing = _maxCardSpacing;
        return;
    }
    float spacing = (this->getContentSize().x - _holderOffset.x) / (cardCount - 1);
    if (_maxCardSpacing > spacing)
        _cardSpacing = spacing;
}

ax::Vec2 Holder::getCardPosition(int index, int cardCount)
{
    float horizontalOffset = (index - (cardCount - 1) / 2.0f) * _cardSpacing;

    float rotationDegrees = this->getRotation();
    float rotationRadians = -AX_DEGREES_TO_RADIANS(rotationDegrees);

    float rotatedX = horizontalOffset * cos(rotationRadians);
    float rotatedY = horizontalOffset * sin(rotationRadians);

    return ax::Vec2(this->getPosition().x + rotatedX, this->getPosition().y + rotatedY);
}
