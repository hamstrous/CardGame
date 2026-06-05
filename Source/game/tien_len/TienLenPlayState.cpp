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
    auto currentSuit = static_cast<TienLenRule::Suit>(game._currentSuit);
    auto currentRank = static_cast<TienLenRule::Rank>(game._currentRank);

    if (clientId != game._currentPlayerId)
        return;
    
    message["type"] = "broadcast";
    message["command"] = "player_response";
    message["time_stamp"] = 0;
    message["data"]       = json::object();

}

void TienLenPlayState::onUpdate(float delta) {}

void TienLenPlayState::onExit() {
    auto& game   = *getContext();
    int clientId = game._clientPlayerId;

    for (auto card : helper::castToVectorOfType<Card*>(game._playerHands.at(clientId)->getChildren()))
    {
        card->lockInput();
    }
    
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
        
    }
}

void TienLenPlayState::onCardClicked(EventCard* event) {
    auto& game   = *getContext();
    int clientId = game._clientPlayerId;

    Card* clickedCard = event->getCard();
    clickedCard->lockInput();
    
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
