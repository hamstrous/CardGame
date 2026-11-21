#pragma once

#include "axmol.h"

#include "core/event/EventMode.h"


class ModeManager : public ax::Node
{
public:
    ModeManager();

    static ModeManager* getInstance();
    static void destroyInstance();

    Mode getCurrentMode() { return currentMode; }

    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);

private:
    static ModeManager* _instance;
    Mode currentMode = Mode::EDIT;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
};
