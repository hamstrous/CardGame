#include "UnoDealState.h"

void UnoDealState::onEnter() {
    auto& game = *getContext();
    game._deck->shuffleCards();
    // deal 7 cards to each player
    game._deck->dealCards(game._playerHands, 7);

}

void UnoDealState::onUpdate(float delta) {}

void UnoDealState::onExit() {}

void UnoDealState::onMouseDown(ax::Event* event) {}

void UnoDealState::onMouseUp(ax::Event* event) {}

void UnoDealState::onMouseMove(ax::Event* event) {}

void UnoDealState::onMouseScroll(ax::Event* event) {}

void UnoDealState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoDealState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoDealState::onWebSocketMessage(EventWebSocket* event) {}
