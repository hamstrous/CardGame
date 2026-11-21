#include "TienLenPlayState.h"
#include "utils/helper.h"
#include "core/event/EventWebsocket.h"

#include "game/tien_len/TienLenResultState.h"

#include "utils/magic_enum.hpp"

void TienLenPlayState::onEnter()
{
    auto& game = *getContext();

    message      = json::object();
    int clientId = game._clientPlayerId;

    if (clientId != game._currentPlayerId)
        return;

    message["type"]       = "broadcast";
    message["command"]    = "player_response";
    message["time_stamp"] = 0;
    message["data"]       = json::object();

    game._playButton->setEnabled(true);
    game._passButton->setEnabled(true);

    game._playButton->addClickEventListener([this](ax::Object* sender) {
        playerPlayCards();
    });
    game._passButton->addClickEventListener([this](ax::Object* sender) {
        playerPass();
    });

    auto hand = helper::castToVectorOfType<Card*>(game._playerHands.at(clientId)->getChildren());

    for (const auto& card : hand)
    {
        card->unlockInput();
    }

}

void TienLenPlayState::onUpdate(float delta) {}

void TienLenPlayState::onExit() {
    auto& game   = *getContext();
    int clientId = game._clientPlayerId;

    for (auto card : helper::castToVectorOfType<Card*>(game._playerHands.at(clientId)->getChildren()))
    {
        card->lockInput();
    }

    game._playButton->setEnabled(false);
    game._passButton->setEnabled(false);
    
}

void TienLenPlayState::onMouseDown(ax::Event* event) {}

void TienLenPlayState::onMouseUp(ax::Event* event) {}

void TienLenPlayState::onMouseMove(ax::Event* event) {}

void TienLenPlayState::onMouseScroll(ax::Event* event) {}

void TienLenPlayState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenPlayState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenPlayState::onWebSocketMessage(EventWebSocket* event)
{
    auto& game = *getContext();
    json data  = event->getData();

    std::string cmd = data["command"];

    if (cmd == "player_response")
    {
        json payload = data["data"];
        // Update game state based on the player's response
        game._currentPlayerId = payload["new_current_player"];
        game._currentCombination = payload["combination"];
        game._currentLargestCardValue = payload["largest_card_value"];
        game._currentConsecutiveCount = payload["consecutive_count"];
        game._currentCombination      = static_cast<TienLenRule::Combination>(payload["combination"]);
        game._currentEndWithTwo = payload["end_with_two"];
        game._currentSameRankCount = payload["same_rank_count"];
        game._recentlyPlayedPlayerId = payload["recently_played_player_id"];
        if (game._recentlyPlayedPlayerId == game._clientPlayerId)
            game._isNewRound = true;
        else
            game._isNewRound = false;

        if (payload.contains("discarded_cards"))
        {
            if (payload["discarded_cards"].get<bool>())
            {
                for (const auto& card : helper::castToVectorOfType<Card*>(game._discardPile->getChildren()))
                {
                    game._discardPile->removeChild(card);
                }
            }
        }
        if (payload.contains("played_cards"))
        {
            auto playedCards = payload["played_cards"];
            for (const auto& cardValue : playedCards)
            {
                int rank = cardValue.get<int>() / 4;
                int suit = cardValue.get<int>() % 4;
                for (auto zone: game._playerHands)
                    for (auto card : helper::castToVectorOfType<Card*>(zone->getChildren()))
                    {
                        if (card->getValue("rank") == rank && card->getValue("suit") == suit)
                        {
                            game._discardPile->moveCardToThisZone(card);
                            card->forceReveal();
                            break;
                        }
                    }
            }
        }
        game.changeState(new TienLenPlayState(getContext()));
    }
}

void TienLenPlayState::onCardClicked(EventCard* event) {
    auto& game   = *getContext();
    int clientId = game._clientPlayerId;

    Card* clickedCard = event->getCard();
    game._playerHands[game._clientPlayerId]->togglePickCard(clickedCard);
    
}

void TienLenPlayState::playerPlayCards() {
    auto& game = *getContext();

    auto& pickedCards = game._playerHands.at(game._clientPlayerId)->getPickedCards();
    if (pickedCards.empty())
    {
        // No card picked, do nothing
        return;
    }

    if (!game.isValidPlay(pickedCards))
    {
        // Invalid play, do nothing or show some warning
        return;
    }
    //game._discardPile->removeAllChildren();`
    message["data"]["discarded_cards"] = false;
    for (const auto& card : helper::castToVectorOfType<Card*>(game._discardPile->getChildren()))
    {
         game._discardPile->removeChild(card);
        message["data"]["discarded_cards"] = true;
    }
    AXLOGD("Picked cards count: {}", pickedCards.size());
    for (auto& card : pickedCards)
    {
        game._discardPile->moveCardToThisZone(card);
        card->lockInput();
    }
    pickedCards.clear();  // Clear picked cards after playing
    // send play card message to server
    setNewCurrentPlayer();
    message["data"]["recently_played_player_id"] = game._clientPlayerId;
    message["data"]["combination"]               = static_cast<int>(game._currentCombination);
    message["data"]["largest_card_value"]        = game._currentLargestCardValue;
    message["data"]["consecutive_count"]         = game._currentConsecutiveCount;
    message["data"]["same_rank_count"]           = game._currentSameRankCount;
    message["data"]["end_with_two"]              = game._currentEndWithTwo;

    // Add played cards to message

    message["data"]["played_cards"] = game._previousPlayedCards;
    message["data"]["discarded_cards"] = true;

    game._socketManager->sendMessage(message);

    game.changeState(new TienLenPlayState(getContext()));
    
}

void TienLenPlayState::playerPass() {
    auto& game = *getContext();
    setNewCurrentPlayer();
    message["data"]["recently_played_player_id"] = game._recentlyPlayedPlayerId;
    message["data"]["combination"]               = static_cast<int>(game._currentCombination);
    message["data"]["largest_card_value"]        = game._currentLargestCardValue;
    message["data"]["consecutive_count"]        = game._currentConsecutiveCount;
    message["data"]["same_rank_count"]         = game._currentSameRankCount;
    message["data"]["end_with_two"]              = game._currentEndWithTwo;
    message["data"]["discarded_cards"]           = false;

    game._socketManager->sendMessage(message);
    game.changeState(new TienLenPlayState(getContext()));
}

void TienLenPlayState::setNewCurrentPlayer() {
    auto& game = *getContext();
    // Change turn
    game._currentPlayerId += (game._clockWise ? 1 : -1);
    if (game._currentPlayerId >= game._playerCount)
        game._currentPlayerId = 0;
    else if (game._currentPlayerId < 0)
        game._currentPlayerId = game._playerCount - 1;
    message["data"]["new_current_player"] = game._currentPlayerId;
}
