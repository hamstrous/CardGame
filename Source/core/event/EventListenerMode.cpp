#include "EventListenerMode.h"


const std::string EventListenerMode::LISTENER_ID = "__ax_mode";

bool EventListenerMode::checkAvailable()
{
    if (false)
    {
        AXASSERT(false, "Invalid EventListenerMode!");
        return false;
    }

    return true;
}

EventListenerMode* EventListenerMode::create()
{
    auto ret = new EventListenerMode();
    if (ret->init())
    {
        ret->autorelease();
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerMode* EventListenerMode::clone()
{
    auto ret = new EventListenerMode();
    if (ret->init())
    {
        ret->autorelease();
    }
    else
    {
        AX_SAFE_DELETE(ret);
    }
    return ret;
}

EventListenerMode::EventListenerMode() : onModeChanged(nullptr) {}

bool EventListenerMode::init()
{
    auto listener = [this](ax::Event* event) {
        auto modeEvent = static_cast<EventMode*>(event);
        if (onModeChanged != nullptr)
            onModeChanged(modeEvent);
    };

    if (EventListenerCustom::init(LISTENER_ID, listener))
    {
        return true;
    }

    return false;
}
