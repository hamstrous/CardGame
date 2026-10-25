#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;

class Inspector
{
public:
    Inspector() = default;

    static Inspector* getInstance();
    static void destroyInstance();

    void draw();

private:
    static Inspector* _instance;
};
