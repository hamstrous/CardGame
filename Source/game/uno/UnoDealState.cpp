#include "UnoDealState.h"

void UnoDealState::onEnter() {
    auto& game = *getContext();
    auto shuffleAction = ax::CallFunc::create([&game]() { game._deck->shuffleCards(); });
    auto dealAction = ax::CallFunc::create([&game]() { game._deck->dealCards(game._playerHands, 7); });

    // deal the player with index 0 first
    auto sequence      = ax::Sequence::create(ax::DelayTime::create(3.0f), shuffleAction, ax::DelayTime::create(5.0f),
                                              dealAction, nullptr);
    game.runAction(sequence);
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
