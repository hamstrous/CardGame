#pragma once

#include "axmol.h"

class InspectorData;

class ContextMenu : public ax::Node
{
public:
    ContextMenu() = default;

    static ContextMenu* getInstance();
    static void destroyInstance();

    void justRightClicked();

    void drawGui();

private:
    bool _justRightClicked = false;
    static ContextMenu* _instance;
};
