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

    game._playButton->setVisible(true);
    game._passButton->setVisible(true);

    game._playButton->addClickEventListener([this](ax::Object* sender) {
        playerPlayCards();
    });
    game._passButton->addClickEventListener([this](ax::Object* sender) {
        playerPass();
    });

    message["type"] = "broadcast";
    message["command"] = "player_response";
    message["time_stamp"] = 0;
    message["data"]       = json::object();

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
    game._playerHands[game._clientPlayerId]->togglePickCard(clickedCard);
    
}

void TienLenPlayState::playerPlayCards() {
    const auto& game = *getContext();
    const auto& hand = game._playerHands.at(getContext()->_clientPlayerId);
    auto& pickedCards = hand->getPickedCards();
    if (pickedCards.empty())
    {
        // No card picked, do nothing
        return;
    }

    if (!getContext()->isValidPlay(pickedCards))
    {
        // Invalid play, do nothing or show some warning
        return;
    }
    //game._discardPile->removeAllChildren();
    for (const auto& card : helper::castToVectorOfType<Card*>(game._discardPile->getChildren()))
    {
         game._discardPile->removeChild(card);
    }
    AXLOGD("Picked cards count: {}", pickedCards.size());
    for (auto& card : pickedCards)
    {
        game._discardPile->moveCardToThisZone(card);
    }
    pickedCards.clear();  // Clear picked cards after playing
    // send play card message to server
}

void TienLenPlayState::playerPass() {}

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
