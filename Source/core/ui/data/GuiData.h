#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;

class GuiData
{
public:
    GuiData() = default;

    virtual void drawGui() = 0;

};
