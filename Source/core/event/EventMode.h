#pragma once

#include "axmol.h"
#include "core/const/GameConstants.h"
#include "core/object/Widget.h"

using namespace constant;

class AX_DLL EventMode : public ax::EventCustom
{
public:
    EventMode(const Mode& mode, const std::set<Widget*>& selectedWidgets = {});
    Mode getMode() const { return _mode; }
    const std::set<Widget*>& getSelectedWidgets() const { return _selectedWidgets; }

private:
    Mode _mode;
    std::set<Widget*> _selectedWidgets; 
    friend class EventListenerMode;
};

