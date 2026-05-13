#pragma once

#include "axmol.h"

#include "core/interface/IState.h"

class EventWebSocket;

class GameState : public IState
{
public:
    virtual void onEnter()             = 0;
    virtual void onUpdate(float delta) = 0;
    virtual void onExit()              = 0;

    // mouse
    virtual void onMouseDown(ax::Event* event)   = 0;
    virtual void onMouseUp(ax::Event* event)     = 0;
    virtual void onMouseMove(ax::Event* event)   = 0;
    virtual void onMouseScroll(ax::Event* event) = 0;

    // Keyboard
    virtual void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event)  = 0;
    virtual void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) = 0;

    // WebSocket
    virtual void onWebSocketMessage(EventWebSocket* event) = 0;

    Rule* getContext() const override { return static_cast<Rule*>(_context); }
};
