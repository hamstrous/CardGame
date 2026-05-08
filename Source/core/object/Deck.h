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
    void moveCardListToTop(ax::Vector<Card*> cardList);

    // Overrides
    void lockInput() override;
    void unlockInput() override;

    // Getters and Setters
    
    // Constructor and Destructor
    ~Deck() override;
};
