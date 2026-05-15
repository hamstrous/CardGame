#pragma once

#include "axmol.h"

#include "core/object/data/ZoneData.h"

#include "Card.h"
#include "Zone.h"

#include "core/event/EventListenerCard.h"
#include "core/event/EventListenerZone.h"
#include "core/event/EventZone.h"

#include "core/interface/ILockableInput.h"

#include "utils/helper.h"


class Deck : public Zone
{
public:

    // Factory method
    static Deck* create(ZoneData* property);
    bool init(ZoneData* property);
    void update(float delta) override;

    // Actions
    void shuffleCards();
    void shuffleCardsWithSeed(std::string seed);
    std::string getRandomShuffleSeed();

    void dealCards(ax::Vector<Zone*> &targetZones, int amount);
    void moveCardListToTop(ax::Vector<Card*> cardList);

    std::vector<ax::Vec2> getCurrentPositionList(ax::Vector<Card*> cardList = ax::Vector<Card*>()) override;

    // Overrides
    void lockInput() override;
    void unlockInput() override;

    // Getters and Setters
    
    // Constructor and Destructor
    ~Deck() override;
};
