#pragma once

#include "axmol.h"


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
