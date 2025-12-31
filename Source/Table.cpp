#include "Table.h"
#include <limits>
#include <algorithm>
using namespace ax;

const ax::Vec2 Table::TABLE_OFFSET{100, 50};
const ax::Vec2 Table::TABLE_SIZE{Card::CARD_SIZE.x * 6 + TABLE_OFFSET.x, Card::CARD_SIZE.y + TABLE_OFFSET.y};

Table* Table::create()
{
    Table* table = new (std::nothrow) Table();
    if (table && table->init())
    {
        table->autorelease();
        return table;
    }
    AX_SAFE_DELETE(table);
    return nullptr;
}

bool Table::init()
{
    if (!Node::init())
        return false;

    _objectSize   = TABLE_SIZE;
    _isDraggable  = false;
    _holderOffset = TABLE_OFFSET;

    _maxCardSpacing = Card::CARD_SIZE.x;
    _cardSpacing    = Card::CARD_SIZE.x;

    Vec2 bottomLeft(-TABLE_SIZE.x / 2, -TABLE_SIZE.y / 2);
    Vec2 topRight(TABLE_SIZE.x / 2, TABLE_SIZE.y / 2);

    Color4F fillColor(0.5686f, 0.9569f, 1.0f, 0.9f);
    ;
    _deckDrawNode = DrawNode::create();
    _deckDrawNode->drawSolidRect(bottomLeft, topRight, fillColor);

    this->addChild(_deckDrawNode);

    _deck = Deck::create();
    _deck->setColor(fillColor);
    setContentSize(TABLE_SIZE);
    // deck will be a child of scene , not table

    return true;
}

Table* Table::clone() const
{
    Table* newTable = new (std::nothrow) Table();
    newTable->autorelease();
    if (newTable)
    {
        newTable->_objectSize     = this->_objectSize;
        newTable->_isDraggable    = this->_isDraggable;
        newTable->_holderOffset   = this->_holderOffset;
        newTable->_maxCardSpacing = this->_maxCardSpacing;
        newTable->_cardSpacing    = this->_cardSpacing;

        Vec2 bottomLeft(-TABLE_SIZE.x / 2, -TABLE_SIZE.y / 2);
        Vec2 topRight(TABLE_SIZE.x / 2, TABLE_SIZE.y / 2);

        Color4F fillColor(0.5686f, 0.9569f, 1.0f, 0.9f);
        ;
        newTable->_deckDrawNode = DrawNode::create();
        newTable->_deckDrawNode->drawSolidRect(bottomLeft, topRight, fillColor);

        newTable->addChild(newTable->_deckDrawNode);

        newTable->_deck = this->_deck;  // shallow copy, deck is shared
        newTable->setContentSize(this->getContentSize());

        return newTable;
    }
    return nullptr;
}

void Table::addCard(Card* card)
{
    for (auto existingCard : _cards)
    {

        _deck->addCard(existingCard);
    }
    _cards.clear();
    card->flip();
    Holder::addCard(card);
}

void Table::addCard(std::vector<Card*>& cards, ax::Vec2& mousePos)
{
    for (auto existingCard : _cards)
    {
        _deck->addCard(existingCard);
    }
    _cards.clear();
    for (auto card : cards)
    {
        card->setFaceUp(true);
    }
    Holder::addCard(cards, mousePos);
}

void Table::setConfig(int id, float posX, float posY, float sizeX, float sizeY, float rotation)
{
    _id = id;
    this->setPosition(ax::Vec2(posX, posY));

    // Update size
    ax::Vec2 newSize(sizeX, sizeY);
    _objectSize = newSize;
    this->setContentSize(ax::Size(sizeX, sizeY));

    // Redraw the table rectangle with new size
    if (_deckDrawNode)
    {
        _deckDrawNode->clear();
        ax::Vec2 bottomLeft(-sizeX / 2, -sizeY / 2);
        ax::Vec2 topRight(sizeX / 2, sizeY / 2);
        ax::Color4F fillColor(0.5686f, 0.9569f, 1.0f, 0.9f);
        _deckDrawNode->drawSolidRect(bottomLeft, topRight, fillColor);
    }

    this->setRotation(rotation);
}
