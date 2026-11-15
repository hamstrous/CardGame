#pragma once

#include "axmol.h"


class GuiManager : public ax::Node
{
public:
    GuiManager() = default;

    static GuiManager* getInstance();
    static void destroyInstance();

    void drawGui();

private:
    static GuiManager* _instance;
};
