#include "EventCard.h"
#include "EventListenerMode.h"

EventMode::EventMode(const Mode& mode, const std::set<Widget*>& selectedWidgets)
    : ax::EventCustom(EventListenerMode::LISTENER_ID), _mode(mode), _selectedWidgets(selectedWidgets)
{}
