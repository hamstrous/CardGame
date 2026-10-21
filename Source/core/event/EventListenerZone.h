#pragma once

#include "axmol.h"
class EventZone; // forward declaration to avoid circular dependency with Zone.h


class AX_DLL EventListenerZone : public ax::EventListenerCustom
{
public:
    static const std::string LISTENER_ID;

    static EventListenerZone* create();

    /// Overrides
    virtual EventListenerZone* clone() override;
    virtual bool checkAvailable() override;

    std::function<void(EventZone*)> onCardReceived = nullptr;

    EventListenerZone();
    bool init();
};
