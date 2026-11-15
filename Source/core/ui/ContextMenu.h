#pragma once

#include "axmol.h"

class InspectorData;

class ContextMenu : public ax::Node
{
public:
    ContextMenu() = default;

    static ContextMenu* getInstance();
    static void destroyInstance();

    void drawGui();

private:
    static ContextMenu* _instance;
};
