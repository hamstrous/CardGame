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
    Deck* clone() const;

    // Set deck configuration from config file
    void setConfig(int id,
                   float posX,
                   float posY,
                   const std::string& colorName,
                   float sizeX,
                   float sizeY,
                   float rotation,
                   int dealAmount);

    // Convert color name to Color4F
    static ax::Color4F getColorFromName(const std::string& colorName);

    void addCard(Card* card) override;
    void addCard(std::vector<Card*> cards);
    void removeCard(Card* card) override;
    void removeCard(std::vector<Card*> card);

    void shuffle();
    void deal();
    void dealSmoothly(float delayPerCard = 0.2f);
    void connectHolder(Holder* holder);
    void connectHolder(const std::vector<Holder*>& holders);
    void disconnectHolder(Holder* holder);
    void disconnectHolder(const std::vector<Holder*>& holders);
    void setConnectedHolders(const std::vector<Holder*>& holders);
    const std::vector<Holder*>& getConnectedHolders() const;

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
    std::vector<Holder*> _connectedHolders;
};
