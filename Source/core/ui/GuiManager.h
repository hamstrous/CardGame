#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;

class GuiManager : public ax::Node
{
public:
    GuiManager() = default;

    static GuiManager* getInstance();
    static void destroyInstance();

    void drawUI();

private:
    static GuiManager* _instance;
};
