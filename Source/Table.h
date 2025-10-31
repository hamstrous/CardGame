#pragma once

#include "axmol.h"
#include "Card.h"
#include "DraggableObject.h"

class Table : public DraggableObject
{
public:
    static Table* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    void addCard(Card* card);
    void removeCard(Card* card);
    void addCardAt(Card* card, int index);
    void moveCardToPosition(Card* card, const ax::Vec2& position);

    // Overloaded functions for vector of cards
    void addCard(const std::vector<Card*>& cards);
    void removeCard(const std::vector<Card*>& cards);
    void addCardAt(const std::vector<Card*>& cards, int index);

    void setSpacing(float spacing)
    {
        if (_cardSpacing > spacing)
            _cardSpacing = spacing;
    }

    void startDragging() override;

    inline static const ax::Vec2 TABLE_SIZE{650, 170};
    inline static const ax::Vec2 TABLE_OFFSET{250, 0};

private:
    ax::Sprite* _tableSprite = nullptr;

    std::vector<Card*> _cards;
    float _cardSpacing = Card::CARD_SIZE.x;
    float _maxCardSpacing;

    ax::Vec2 getCardPosition(int index, int cardCount) const;
};
