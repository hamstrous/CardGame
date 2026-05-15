#include "UnoDealState.h"
#include "utils/json.hpp"

#include "core/event/EventWebsocket.h"

using json = lib::json;

void UnoDealState::onEnter() {
    auto& game = *getContext();

    if (game._isHost)
    {
        shuffleSeed = game._deck->getRandomShuffleSeed();
        game._socketManager->sendMessage(
            json {
                {"type","broadcast"},
                {"command", "shuffle"},
                {"data", json{{"shuffleSeed", shuffleSeed}}},
                {"time_stamp", 0}
            }
        );
    }

    if (shuffleSeed.empty())
    {
        AXLOGD("Waiting for shuffle seed from host...");
        return;
    }

    auto shuffleAction = ax::CallFunc::create([shuffleSeed = this->shuffleSeed, &game]() { game._deck->shuffleCardsWithSeed(shuffleSeed); });
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

void UnoDealState::onWebSocketMessage(EventWebSocket* event) {
    auto& game = *getContext();

    json data = event->getData();

    if (data["command"] == "shuffle")
    {
        shuffleSeed = data["data"]["shuffleSeed"];
        AXLOGD("Received shuffle seed from host: {}", shuffleSeed);

        // Try shuffle and deal again
        onEnter();
    }
}
