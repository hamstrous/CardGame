#pragma once

#include "axmol.h"
#include "Card.h"
#include "DraggableObject.h"

class Holder : public DraggableObject
{
public:
    virtual void addCard(Card* card);
    virtual void removeCard(Card* card);
    virtual void addCardAt(Card* card, int index);
    virtual void moveCardToPosition(Card* card, const ax::Vec2& position);

    // Overloaded functions for vector of cards
    virtual void addCard(std::vector<Card*>& cards, ax::Vec2& mousePos);
    virtual void removeCard(const std::vector<Card*>& cards);
    virtual void addCardAt(std::vector<Card*>& cards, int index);

    virtual void setSpacing(int cardCount);

    void startDragging() override;

protected:
    std::vector<Card*> _cards;
    float _cardSpacing    = 82.0f;  // Default to Card::CARD_SIZE.x
    float _maxCardSpacing = 82.0f;  // Default to Card::CARD_SIZE.x
    ax::Vec2 _holderOffset{250, 0};

    virtual ax::Vec2 getCardPosition(int index, int cardCount);
};
