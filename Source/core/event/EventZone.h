#pragma once

#include "axmol.h"
#include "core/object/Zone.h"

class AX_DLL EventZone : public ax::EventCustom
{
public:
    EventZone();

private:
    Zone* zone;  // The zone associated with this event
    Card* card;  // The card associated with this event, if any
    //ax::Vector<Card*> cardList;

    bool _isReceived;
    bool _isRemoved;
    friend class EventListenerZone;
};

