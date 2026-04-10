#pragma once

#include "axmol.h"

#include "ZoneData.h"
#include "Card.h"

#include "utils/helper.h"


class Zone : public ax::Node
{
public:
    // Factory method
    static Zone* create(ZoneData* property);
    bool init(ZoneData* property);

    // Event handlers
    bool onMouseDown(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    // Actions
    void shuffleCards();
    void sendCardToAnotherZone(Zone* targetZone, Card* card);
    void sortCards();
    void moveCardToThisZone(Card* card, float duration = 1.f);
    void getNewCardIndex(Card* card); // Card is not in this zone yet
    void getNewCardPosition(Card* card);

    // Getters and Setters
    
    // Constructor and Destructor
    ~Zone() override;

protected:
    ax::Vector<Card*> _cardList;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;

};
