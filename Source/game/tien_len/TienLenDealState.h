#pragma once

#include "axmol.h"

#include "TienLenState.h"
#include "TienLenRule.h"

class TienLenRule;

class TienLenDealState : public TienLenState
{
public:
    TienLenDealState(TienLenRule* context) : TienLenState(context) {}

    // Inherited via TienLenState
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

protected:
    std::string shuffleSeed;
};
