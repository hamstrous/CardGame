#pragma once

#include "axmol.h"

#include "GuiData.h"

class ContextMenuData : public GuiData
{

public:
    // Helper

public:
    ContextMenuData() = default;

    std::function<void()> duplicateCallback = nullptr;
    std::function<void()> deleteCallback = nullptr;

};
