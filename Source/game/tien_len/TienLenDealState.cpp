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

    auto setFirstCardAction = ax::CallFunc::create([this, &game]() {
        for (int i = game._deck->getChildren().size() - 1; i > 0; i--)
        {
            auto card = dynamic_cast<Card*> (game._deck->getChildren().at(i));
            if (!isCardSpecial(card))
            {
                game._currentColor = static_cast<TienLenRule::Color>(card->getValue("color"));
                game._currentValue = static_cast<TienLenRule::Value>(card->getValue("value"));
                game._discardPile->moveCardToThisZone(card);
                return;
            }
        }
    });

    auto playGame = ax::CallFunc::create([this, &game]() { game.changeState(new TienLenPlayState(getContext())); });

    auto sequence      = ax::Sequence::create(ax::DelayTime::create(3.0f), shuffleAction, ax::DelayTime::create(5.0f), dealAction,
                             ax::DelayTime::create(6.0f), setFirstCardAction, playGame, nullptr);
    game.runAction(sequence);
}

void TienLenDealState::onUpdate(float delta) {}

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
