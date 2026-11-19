#pragma once

#include "axmol.h"


class GuiManager : public ax::Node
{
public:
    GuiManager() = default;

    static GuiManager* getInstance();
    static void destroyInstance();

    void addCallbackToImGuiDrawLoop(std::function <void()> callback);

    void drawGui();

private:
    static GuiManager* _instance;
    std::vector<std::function<void()>> _imGuiDrawCallbackList;
};
