#include "TienLenResultState.h"

void TienLenResultState::onEnter() {
    //Clear the screen
    auto& game = *getContext();
    game.removeAllChildrenWithCleanup(true);
    auto resultText = ax::ui::Text::create(_resultText, "Arial", 60);
    resultText->setPosition(game.visibleSize / 2);
    game.addChild(resultText);
}

void TienLenResultState::onUpdate(float delta) {}

void TienLenResultState::onExit() {}

void TienLenResultState::onMouseDown(ax::Event* event) {}

void TienLenResultState::onMouseUp(ax::Event* event) {}

void TienLenResultState::onMouseMove(ax::Event* event) {}

void TienLenResultState::onMouseScroll(ax::Event* event) {}

void TienLenResultState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenResultState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenResultState::onWebSocketMessage(EventWebSocket* event) {}
