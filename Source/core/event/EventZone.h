#pragma once

#include "axmol.h"
#include "core/object/Zone.h"
#include "core/object/Card.h"

class AX_DLL EventZone : public ax::EventCustom
{
public:
    EventZone(Zone* zone, Card* card = nullptr);

    Card* getCard() const { return _card; }
    Zone* getZone() const { return _zone; }

private:
    Zone* _zone;  // The zone associated with this event
    Card* _card;  // The card associated with this event, if any
    //ax::Vector<Card*> cardList;

    bool _isReceived;
    bool _isRemoved;
    friend class EventListenerZone;
};

