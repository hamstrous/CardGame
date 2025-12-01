#pragma once

#include "axmol.h"
#include "Card.h"
#include "Holder.h"
#include "Rack.h"
#include "ui/UIButton.h"

class Deck : public Holder
{
public:
    static Deck* create();

    bool init(const ax::Color4F& color = ax::Color4F(0.7f, 0.7f, 0.7f, 0.9f));

    void addCard(Card* card) override;
    void addCard(std::vector<Card*> cards);
    void removeCard(Card* card) override;
    void removeCard(std::vector<Card*> card);

    void shuffle();
    void deal(std::vector<Rack*>& racks);
    void dealSmoothly(std::vector<Rack*>& racks, float delayPerCard = 0.2f);

    void setColor(const ax::Color4F& color);

    static const ax::Vec2 DECK_OFFSET;
    static const ax::Vec2 DECK_SIZE;

private:
    int _dealAmount                  = 1;
    ax::DrawNode* _deckDrawNode      = nullptr;
    ax::ui::Button* _incrementButton = nullptr;
    ax::ui::Button* _decrementButton = nullptr;
    ax::ui::Button* _resetButton     = nullptr;
    ax::Label* _dealAmountLabel      = nullptr;
};
