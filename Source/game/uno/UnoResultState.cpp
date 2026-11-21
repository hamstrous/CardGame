#include "UnoResultState.h"

void UnoResultState::onEnter() {
    //Clear the screen
    auto& game = *getContext();
    game.removeAllChildrenWithCleanup(true);
    auto resultText = ax::ui::Text::create(_resultText, "Arial", 60);
    resultText->setPosition(game.visibleSize / 2);
    game.addChild(resultText);
}

void UnoResultState::onUpdate(float delta) {}

void UnoResultState::onExit() {}

void UnoResultState::onMouseDown(ax::Event* event) {}

void UnoResultState::onMouseUp(ax::Event* event) {}

void UnoResultState::onMouseMove(ax::Event* event) {}

void UnoResultState::onMouseScroll(ax::Event* event) {}

void UnoResultState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoResultState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoResultState::onWebSocketMessage(EventWebSocket* event) {}
