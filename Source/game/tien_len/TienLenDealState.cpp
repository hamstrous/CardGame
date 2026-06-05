#include "TienLenDealState.h"
#include "utils/json.hpp"

#include "core/event/EventWebsocket.h"
#include "game/tien_len/TienLenPlayState.h"

using json = nlohmann::json;

void TienLenDealState::onEnter() {
    auto& game = *getContext();

    if (game._isHost)
    {
        shuffleSeed = game._deck->getRandomShuffleSeed();
        game._socketManager->sendMessage(
            json {
                {"type","broadcast"},
                {"command", "shuffle"},
                {"data", json{{"shuffle_seed", shuffleSeed}}},
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

    auto dealAction = ax::CallFunc::create([&game]() { game._deck->dealCards(game._playerHands, 2); });
    // deal the player with index 0 first

    auto playGame = ax::CallFunc::create([this, &game]() { game.changeState(new TienLenPlayState(getContext())); });

    auto sequence      = ax::Sequence::create(ax::DelayTime::create(3.0f), shuffleAction, ax::DelayTime::create(5.0f), dealAction,
                             ax::DelayTime::create(6.0f), playGame, nullptr);
    game.runAction(sequence);
}

void TienLenDealState::onUpdate(float delta) {
    auto& game = *getContext();
    for (auto card : helper::castToVectorOfType<Card*>(game._playerHands[game._clientPlayerId]->getChildren()))
    {
        card->forceReveal();
    }
}

void TienLenDealState::onExit() {}

void TienLenDealState::onMouseDown(ax::Event* event) {}

void TienLenDealState::onMouseUp(ax::Event* event) {}

void TienLenDealState::onMouseMove(ax::Event* event) {}

void TienLenDealState::onMouseScroll(ax::Event* event) {}

void TienLenDealState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenDealState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenDealState::onWebSocketMessage(EventWebSocket* event) {
    auto& game = *getContext();

    json data = event->getData();

    if (data["command"] == "shuffle")
    {
        shuffleSeed = data["data"]["shuffle_seed"];
        AXLOGD("Received shuffle seed from host: {}", shuffleSeed);

        // Try shuffle and deal again
        onEnter();
    }
}
