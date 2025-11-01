#pragma once

#include "axmol.h"
#include "Card.h"
#include "DraggableObject.h"

class Rack : public DraggableObject
{
public:
    static Rack* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    void addCard(Card* card);
    void removeCard(Card* card);
    void addCardAt(Card* card, int index);
    void moveCardToPosition(Card* card, const ax::Vec2& position);

    // Overloaded functions for vector of cards
    void addCard(std::vector<Card*>& cards, ax::Vec2& mousePos);
    void removeCard(const std::vector<Card*>& cards);
    void addCardAt(std::vector<Card*>& cards, int index);

    void setSpacing(int cardCount)
    {
        if (cardCount <= 1)
        {
            _cardSpacing = _maxCardSpacing;
            return;
        }
        float spacing = (this->getContentSize().x - TABLE_OFFSET.x) / (cardCount - 1);
        if (_maxCardSpacing > spacing)
            _cardSpacing = spacing;
    }

    void startDragging() override;

    inline static const ax::Vec2 TABLE_SIZE{650, 170};
    inline static const ax::Vec2 TABLE_OFFSET{250, 0};

private:
    ax::Sprite* _tableSprite = nullptr;

    std::vector<Card*> _cards;
    float _cardSpacing = Card::CARD_SIZE.x;
    const float _maxCardSpacing = Card::CARD_SIZE.x;

    ax::Vec2 getCardPosition(int index, int cardCount);
};
