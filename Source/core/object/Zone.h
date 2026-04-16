#pragma once

#include "axmol.h"

#include "core/object/data/ZoneData.h"

#include "Card.h"

#include "core/event/EventListenerCard.h"

#include "utils/helper.h"


class Zone : public ax::Node
{
public:
    // Factory method
    static Zone* create(ZoneData* property);
    bool init(ZoneData* property);
    void update(float delta) override;

    // Event handlers
    bool onMouseDown(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    void OnCardMouseUp(ax::Event* event);

    // Actions
    void moveCard(Card* card, const ax::Vec2& targetPosition, float duration = 1.f);
    void shuffleCards();
    void sendCardToAnotherZone(Zone* targetZone, Card* card);
    void sortCards();
    void moveCardToThisZone(Card* card, float duration = 1.f);
    void getNewCardIndex(Card* card); 
    void getNewCardPosition(Card* card);
    std::vector<ax::Vec2> getCurrentPositionList();

    // Overrides
    void setContentSize(const ax::Size& contentSize) override;

    // Getters and Setters
    
    // Constructor and Destructor
    ~Zone() override;

protected:
    ax::DrawNode* _rectNode = nullptr;

    ax::Vector<Card*> _cardList;  // currently using get all children and filter by tag

    // Events
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    EventListenerCard* _cardListener = nullptr;

};
