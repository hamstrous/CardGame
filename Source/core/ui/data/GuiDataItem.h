#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;

class GuiDataItem
{
public:
    GuiDataItem() = default;

    virtual void drawGuiItem();

};
