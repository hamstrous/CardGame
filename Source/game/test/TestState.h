#pragma once

#include "axmol.h"

#include "core/rule/GameState.h"

#include "TestRule.h"

class TestState : public GameState
{
public:
    TestState(TestRule* context) : GameState(context) {}

    TestRule* getContext() { return static_cast<TestRule*>(_context); }

protected:
    // Inherited via GameState
    void onEnter() override;
    void onUpdate(float delta) override;
    void onExit() override;
    void onMouseDown(ax::Event* event) override;
    void onMouseUp(ax::Event* event) override;
    void onMouseMove(ax::Event* event) override;
    void onMouseScroll(ax::Event* event) override;
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) override;
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) override;
    void onWebSocketMessage(EventWebSocket* event) override;
};
