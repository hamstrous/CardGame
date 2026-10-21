#pragma once

#include "axmol.h"

#include "core/object/data/ZoneData.h"

#include "Card.h"

#include "core/event/EventListenerCard.h"
#include "core/event/EventListenerZone.h"
#include "core/event/EventZone.h"

#include "core/interface/ILockableInput.h"

#include "utils/helper.h"


class Zone : public ax::Node, public ILockableInput
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
    virtual std::vector<ax::Vec2> getCurrentPositionList(ax::Vector<Card*> cardList = ax::Vector<Card*>()); //List of positions for cards in this zone, used to update card positions when a change happens
    ax::TargetedAction* moveCard(Card* card, const ax::Vec2& targetPosition, float duration = 1.f);
    void sendCardToAnotherZone(Zone* targetZone, Card* card);
    void sortCards();
    void moveCardToThisZone(Card* card, float duration = 1.f);
    void moveCardToThisZone(ax::Vector<Card*> cards, float duration = 1.f);
    void getNewCardIndex(Card* card); 
    void getNewCardPosition(Card* card);
    void resetZoneCardPosition(float duration = 1.f);

    void sortCardsByValue(std::function<int(const Card*)>);  // Example sorting method, can be modified to sort by different criteria

    // Picking cards
    void togglePickCard(Card* card);
    void pickCard(Card* card);
    void unpickCard(Card* card);
    std::set<Card*>& getPickedCards() { return _pickedCards; }

    // Overrides
    void setContentSize(const ax::Size& contentSize) override;
    virtual void lockInput() override;
    virtual void unlockInput() override;
    // Getters and Setters
    
    // Constructor and Destructor
    ~Zone() override;

protected:
    ax::DrawNode* _rectNode = nullptr;

    ax::Vector<Card*> _cardList;  // currently using get all children and filter
    std::set<Card*> _pickedCards;  // use set for easy add/remove and check if a card is picked

    // Events
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    EventListenerCard* _cardListener = nullptr;

};
