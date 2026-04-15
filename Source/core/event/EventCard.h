#pragma once

#include "axmol.h"
#include "core/object/Card.h"

class AX_DLL EventCard : public ax::EventCustom
{
public:
    EventCard(Card* card, bool isFlipped);

    bool isFlipped() const { return _isCardFlipped; }

private:
    Card* card;  // The card associated with this event
    bool _isCardFlipped;  // Whether the card is flipped or not

    friend class EventListenerCard;
};

