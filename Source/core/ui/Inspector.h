#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;

class Inspector : public ax::Node
{
public:
    Inspector() = default;

    static Inspector* getInstance();
    static void destroyInstance();

    void drawUI();

private:
    static Inspector* _instance;
};
