#pragma once

#include "axmol.h"
#include "Card.h"
#include "Deck.h"
#include "Holder.h"
#

class Table : public Holder
{
public:
    static Table* create();

    virtual bool init();
    Table* clone() const;

    // Set table configuration from config file
    void setConfig(int id, float posX, float posY, float sizeX, float sizeY, float rotation);

    void addCard(Card* card) override;
    void addCard(std::vector<Card*>& cards, ax::Vec2& mousePos) override;

    static const ax::Vec2 TABLE_SIZE;
    static const ax::Vec2 TABLE_OFFSET;

    Deck* getDiscardDeck() const { return _deck; }
    void clearDiscardDeck()
    {
        if (_deck)
            _deck = nullptr;
    };

private:
    ax::DrawNode* _deckDrawNode = nullptr;
    Deck* _deck                 = nullptr;  // discard deck
};
