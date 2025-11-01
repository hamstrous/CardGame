#pragma once

#include "axmol.h"
#include "Card.h"
#include "Holder.h"
#include "Rack.h"

class Deck : public Holder
{
public:
    static Deck* create();

    virtual bool init();

    void addCard(Card* card) override;
    void addCard(std::vector<Card*> cards);
    void removeCard(Card* card) override;
    void removeCard(std::vector<Card*> card);

    void shuffle();
    void deal(int count, std::vector<Rack*>& racks);

    static const ax::Vec2 DECK_OFFSET;
    static const ax::Vec2 DECK_SIZE;

private:
    ax::DrawNode* _deckDrawNode = nullptr;
};
