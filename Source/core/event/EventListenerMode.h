#pragma once

#include "axmol.h"
#include "EventMode.h"


class AX_DLL EventListenerMode : public ax::EventListenerCustom
{
public:
    static const std::string LISTENER_ID;

    static EventListenerMode* create();

    /// Overrides
    virtual EventListenerMode* clone() override;
    virtual bool checkAvailable() override;

    std::function<void(EventMode*)> onModeChanged;
    EventListenerMode();
    bool init();
};
