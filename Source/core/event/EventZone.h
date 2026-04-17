#pragma once

#include "axmol.h"
#include "core/object/Card.h"

class AX_DLL EventCard : public ax::EventCustom
{
public:
    EventCard(Card* card, bool isFlipped);
    EventCard(Card* card, ax::Vec2 releasePosition);

    bool isFlipped() const { return _isCardFlipped; }
    ax::Vec2 getReleasePosition() const { return _releasePosition; }
    Card* getCard() const { return card; }

private:
    Card* card;  // The card associated with this event
    bool _isCardFlipped;  // Whether the card is flipped or not
    ax::Vec2 _releasePosition;  // Position where the card was released (for drag-and-drop)

    friend class EventListenerCard;
};

