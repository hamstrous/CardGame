#pragma once

#include "axmol.h"
#include "EventCard.h"


class AX_DLL EventListenerCard : public ax::EventListenerCustom
{
public:
    static const std::string LISTENER_ID;

    static EventListenerCard* create();

    /// Overrides
    virtual EventListenerCard* clone() override;
    virtual bool checkAvailable() override;

    std::function<void(EventCard*)> onCardFlipped;
    std::function<void(EventCard*)> onCardReleased;  // Mouse up after dragging a card
    EventListenerCard();
    bool init();
};
