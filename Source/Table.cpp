#include "Table.h"
#include <limits>
using namespace ax;

Table* Table::create(const std::string& texture)
{
    Table* table = new (std::nothrow) Table();
    if (table && table->init(texture))
    {
        table->autorelease();
        return table;
    }
    AX_SAFE_DELETE(table);
    return nullptr;
}

bool Table::init(const std::string& texture)
{
    // Always call parent init first!
    if (!Node::init())
        return false;

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

void Table::addCard(Card* card)
{
    // If no cards exist, add at position 0
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

    // Also check the position after the last card
    Vec2 lastPos        = getCardPosition(_cards.size(), _cards.size() + 1);
    float distanceToEnd = cardPos.distance(lastPos);
    if (distanceToEnd < minDistance)
    {
        nearestIndex = _cards.size();
    }

    addCardAt(card, nearestIndex);
}

void Table::removeCard(Card* card)
{
    auto it = std::find(_cards.begin(), _cards.end(), card);
    if (it != _cards.end())
    {
        int index = std::distance(_cards.begin(), it);
        _cards.erase(it);
        // Reposition remaining cards
        for (int i = 0; i < _cards.size(); i++)
        {
            moveCardToPosition(_cards[i], getCardPosition(i, _cards.size()));
        }
    }
}

void Table::addCardAt(Card* card, int index)
{
    for (int i = 0; i < index; i++)
    {
        moveCardToPosition(_cards[i], getCardPosition(i, _cards.size() + 1));
    }
    for (int i = index; i < _cards.size(); i++)
    {
        moveCardToPosition(_cards[i], getCardPosition(i + 1, _cards.size() + 1));
    }
    _cards.insert(_cards.begin() + index, card);
    moveCardToPosition(card, getCardPosition(index, _cards.size()));
}

void Table::moveCardToPosition(Card* card, const ax::Vec2& position)
{
    // Stop all previous actions to prevent interference
    card->stopAllActions();

    auto moveTo  = MoveTo::create(0.3f, position);
    auto easeOut = EaseOut::create(moveTo, 2.0f);
    card->runAction(easeOut);
}

bool Table::containsPoint(const ax::Vec2& worldPoint) const
{
    Vec2 localPoint = this->convertToNodeSpace(worldPoint);
    Rect bbox       = Rect(-this->getContentSize().width / 2, -this->getContentSize().height / 2,
                           this->getContentSize().width, this->getContentSize().height);
    if (bbox.containsPoint(localPoint))
        return true;
    return false;
}

void Table::enableDragging(bool enable)
{
    _isDraggable = enable;
}

void Table::setHighlight(bool highlight)
{

    if (highlight && !_highlightNode)
    {
        _highlightNode = ax::DrawNode::create();
        Vec2 rect[4];
        float w = this->getContentSize().width;
        float h = this->getContentSize().height;
        rect[0] = Vec2(-w / 2, -h / 2);
        rect[1] = Vec2(w / 2, -h / 2);
        rect[2] = Vec2(w / 2, h / 2);
        rect[3] = Vec2(-w / 2, h / 2);
        _highlightNode->drawPoly(rect, 4, true, Color4F::YELLOW);
        this->addChild(_highlightNode, 100);
        this->setScale(_originalScale * 1.05f);
    }
    else if (!highlight && _highlightNode)
    {
        this->removeChild(_highlightNode);
        _highlightNode = nullptr;
        this->setScale(_originalScale);
    }
}

ax::Vec2 Table::getCardPosition(int index, int cardCount) const
{
    return ax::Vec2(this->getPosition().x - (cardCount - 1) * _cardSpacing / 2 + index * _cardSpacing,
                    this->getPosition().y);
}
