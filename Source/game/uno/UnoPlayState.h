#pragma once

#include "axmol.h"

#include "UnoState.h"
#include "UnoRule.h"

#include "utils/helper.h"

using json = nlohmann::json;


class UnoPlayState : public UnoState
{
public:
    UnoPlayState(UnoRule* context) : UnoState(context) {}

    // Inherited via UnoState
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

    void onCardClicked(EventCard* event) override;

    void setNewCurrentPlayer();

protected:
    json message;
};
