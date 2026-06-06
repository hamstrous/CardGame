#include "TienLenDealState.h"
#include "utils/json.hpp"

#include "core/event/EventWebsocket.h"
#include "utils/helper.h"
#include "game/tien_len/TienLenPlayState.h"
#include "game/tien_len/TienLenResultState.h"

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

    auto dealAction = ax::CallFunc::create([&game]() { game._deck->dealCards(game._playerHands, 40); });

    // Check who go first
    auto startCondition = ax::CallFunc::create([&game]() {
        // find the player who has smallest card (3 of clubs) and set them as the current player
        int currentPlayerId = 0;
        int smallestRank = std::numeric_limits<int>::max();
        for (int i = 0; i < game._playerHands.size(); ++i)
        {
            auto hand = game._playerHands[i];
            auto cardsInHand = helper::castToVectorOfType<Card*>(hand->getChildren());
            for (auto card : cardsInHand)
            {
                int cardValue = card->getValue("rank") * 4 + card->getValue("suit");
                AXLOGD("Player {} has card with rank {} and suit {}, value: {}", i, card->getValue("rank"),
                       card->getValue("suit"), cardValue);
                if (cardValue < smallestRank)
                {
                    smallestRank = cardValue;
                    game._currentPlayerId = i;
                }
            }
        }
        if (game._currentPlayerId == game._clientPlayerId)
            game._isNewRound = true;
        return true;
    });

    auto checkInstantWinCondition = ax::CallFunc::create([this, &game] {
        bool hasInstantWinHand = false;
        bool playerHasInstantWinHand = false;
        for (int i = 0; i < game._playerCount; i++)
        {
            if (game.isWinHand(helper::castToVectorOfType<Card*>(game._playerHands[i]->getChildren())))
            {
                hasInstantWinHand = true;
                if (i == game._clientPlayerId)
                {
                    playerHasInstantWinHand = true;
                    break;
                }
            }
        }
        if (playerHasInstantWinHand)
        {
            game.changeState(new TienLenResultState(getContext(), "You win"));
        }
        else if (hasInstantWinHand)
        {
            game.changeState(new TienLenResultState(getContext(), "You lose"));
        }
        else
        {
            game.changeState(new TienLenPlayState(getContext()));
        }
        return true;
        }
    );

    auto sequence      = ax::Sequence::create(ax::DelayTime::create(3.0f), shuffleAction, ax::DelayTime::create(5.0f),
        dealAction, ax::DelayTime::create(36.0f), startCondition, checkInstantWinCondition, nullptr);
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
