#include "UnoPlayState.h"
#include "utils/helper.h"
#include "core/event/EventWebsocket.h"

#include "game/uno/UnoResultState.h"

#include "utils/magic_enum.hpp"

void UnoPlayState::onEnter()
{
    auto& game = *getContext();

    message      = json::object();
    int clientId = game._clientPlayerId;
    auto currentColor = static_cast<UnoRule::Color>(game._currentColor);
    auto currentValue = static_cast<UnoRule::Value>(game._currentValue);

    if (clientId != game._currentPlayerId)
        return;
    
    message["type"] = "broadcast";
    message["command"] = "player_response";
    message["time_stamp"] = 0;
    message["data"]       = json::object();

    bool canPlayCard = true;
    if (game._skipNextPlayer)
    {
        message["data"]["new_skip"] = false;
        game._skipNextPlayer        = false;
        canPlayCard                 = false;
        setNewCurrentPlayer();
        game._socketManager->sendMessage(message);
        game.changeState(new UnoPlayState(getContext()));
        return;
    }
    if (game._currentValue == UnoRule::Value::WILD_DRAW_FOUR && game._plus4Stacked)
    {
        game._plus4Stacked = false;
        // get 4 id of top deck
        auto tempHand = ax::Vector<Zone*>();
        tempHand.pushBack(game._playerHands.at(clientId));
        auto cardIds = vector<int>();
        for (int i = game._deck->getChildren().size() - 1; i >= 0 && cardIds.size() < 4; i--)
        {
            auto card = dynamic_cast<Card*>(game._deck->getChildren().at(i));
            if (card == nullptr)
                continue;
            cardIds.push_back(card->getId());
        }
        game._deck->dealCards(tempHand, 4);
        message["data"]["card_moved"]["card_ids"] = cardIds;
        message["data"]["card_moved"]["new_zone_id"]  = clientId;
        message["data"]["new_plus4_stacked"]         = false;
        canPlayCard                               = false;
        setNewCurrentPlayer();
        game._socketManager->sendMessage(message);
        game.changeState(new UnoPlayState(getContext()));
        return;
    }
    // Open input for the card that player can play
    bool hasPlayableCard = false;
    if (canPlayCard)
        for (auto card : helper::castToVectorOfType<Card*>(game._playerHands.at(clientId)->getChildren()))
        {
            auto cardColor = static_cast<UnoRule::Color>(card->getValue("color"));
            auto cardValue    = static_cast<UnoRule::Value>(card->getValue("value"));
            if ((cardColor == currentColor && game._plus2StackCount == 0) || cardColor == UnoRule::Color::WILD || cardValue == currentValue)
            {
                card->unlockInput();
                hasPlayableCard = true;
            }
        }
    if (!hasPlayableCard && (currentValue == UnoRule::Value::DRAW_TWO && game._plus2StackCount > 0))
    {
        // Draw stack x 2 cards
        auto tempHand = ax::Vector<Zone*>();
        tempHand.pushBack(game._playerHands.at(clientId));
        auto cardIds = vector<int>();
        for (int i = game._deck->getChildren().size() - 1; i >= 0 && cardIds.size() < game._plus2StackCount * 2; i--)
        {
            auto card = dynamic_cast<Card*>(game._deck->getChildren().at(i));
            if (card == nullptr)
                continue;
            cardIds.push_back(card->getId());
        }
        game._deck->dealCards(tempHand, game._plus2StackCount * 2);
        message["data"]["card_moved"]["card_ids"] = cardIds;
        message["data"]["card_moved"]["new_zone_id"]  = clientId;
        game._plus2StackCount                         = 0;
        message["data"]["new_plus2_stack_count"]      = 0;
        setNewCurrentPlayer();
        game._socketManager->sendMessage(message);
        game.changeState(new UnoPlayState(getContext()));
    }
    else if (!hasPlayableCard)
    {
        //auto tempHand = ax::Vector<Zone*>();
        //tempHand.pushBack(game.);
        auto cardIds = vector<int>();
        cardIds.push_back(dynamic_cast<Card*>(game._deck->getChildren().back())->getId());
        game._playerHands.at(clientId)->moveCardToThisZone(dynamic_cast<Card*>(game._deck->getChildren().back()));
        message["data"]["card_moved"]["card_ids"] = cardIds;
        message["data"]["card_moved"]["new_zone_id"]  = clientId;
        message["data"]["new_plus2_stack_count"]  = 0;
        setNewCurrentPlayer();
        game._socketManager->sendMessage(message);
        game.changeState(new UnoPlayState(getContext()));

    }
}

void UnoPlayState::onUpdate(float delta) {}

void UnoPlayState::onExit() {
    auto& game   = *getContext();
    int clientId = game._clientPlayerId;

    for (auto card : helper::castToVectorOfType<Card*>(game._playerHands.at(clientId)->getChildren()))
    {
        card->lockInput();
    }
    
}

void UnoPlayState::onMouseDown(ax::Event* event) {}

void UnoPlayState::onMouseUp(ax::Event* event) {}

void UnoPlayState::onMouseMove(ax::Event* event) {}

void UnoPlayState::onMouseScroll(ax::Event* event) {}

void UnoPlayState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoPlayState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoPlayState::onWebSocketMessage(EventWebSocket* event)
{
    auto& game = *getContext();
    json data  = event->getData();

    std::string cmd = data["command"];

    if (cmd == "player_response")
    {
        json payload = data["data"];
        if (payload.contains("card_moved"))
        {
            auto cardIds = payload["card_moved"]["card_ids"].get<vector<int>>();
            int zoneId = payload["card_moved"]["new_zone_id"];
            // zone = -1 for discard pile, otherwise it's player hand with index = zoneId
            if (zoneId == -1)
            {
                for (auto card : helper::castToVectorOfType<Card*>(game._cards))
                {
                    for (int id : cardIds)
                        if (card->getId() == id)
                        {
                            game._discardPile->moveCardToThisZone(card);
                            break;
                        }
                }
            }
            else
            {
                for (auto card : helper::castToVectorOfType<Card*>(game._cards))
                {
                    for (int id : cardIds)
                        if (card->getId() == id)
                        {
                            game._playerHands.at(zoneId)->moveCardToThisZone(card);
                            break;
                        }
                }
            }
        }

        if (payload.contains("new_current_player"))
        {
            int newCurrentPlayer  = payload["new_current_player"];
            game._currentPlayerId = newCurrentPlayer;
        }

        if (payload.contains("new_order"))
        {
            game._clockWise = payload["new_order"];
        }

        if (payload.contains("new_skip"))
        {
            game._skipNextPlayer = payload["new_skip"];
        }

        if (payload.contains("new_current_color"))
        {
            game._currentColor = static_cast<UnoRule::Color>(payload["new_current_color"]);
        }

        if (payload.contains("new_current_value"))
        {
            game._currentValue = static_cast<UnoRule::Value>(payload["new_current_value"]);
        }

        if (payload.contains("new_plus2_stack_count"))
        {
            game._plus2StackCount = payload["new_plus2_stack_count"];
        }
        if (payload.contains("new_plus4_stacked"))
        {
            game._plus4Stacked = payload["new_plus4_stacked"];
        }
        game.changeState(new UnoPlayState(getContext()));
    }
}

void UnoPlayState::onCardClicked(EventCard* event) {
    auto& game   = *getContext();
    int clientId = game._clientPlayerId;

    Card* clickedCard = event->getCard();
    clickedCard->lockInput();
    // Move the card to the discard pile
    game._discardPile->moveCardToThisZone(clickedCard);
    game._playerHands.at(clientId)->resetZoneCardPosition();

    message["data"]["card_moved"]["card_ids"] = vector<int>{clickedCard->getId()};
    message["data"]["card_moved"]["new_zone_id"] = -1;

    auto cardColor = static_cast<UnoRule::Color>(clickedCard->getValue("color"));
    auto cardValue = static_cast<UnoRule::Value>(clickedCard->getValue("value"));

    game._currentColor = cardColor;
    game._currentValue = cardValue;

    message["data"]["new_current_color"] = cardColor;
    message["data"]["new_current_value"] = cardValue;

    int oldPlus2StackCount = game._plus2StackCount;
    game._plus2StackCount  = 0;  // reset the +2 stack count if the played card is not a +2 card

    switch (cardValue)
    {
        case UnoRule::Value::DRAW_TWO:
            message["data"]["new_plus2_stack_count"] = oldPlus2StackCount + 1;
            game._plus2StackCount = oldPlus2StackCount + 1;
            break;
        case UnoRule::Value::SKIP:
            message["data"]["new_skip"] = true;
            game._skipNextPlayer        = true;
            break;
        case UnoRule::Value::REVERSE:
            message["data"]["new_order"] = !game._clockWise;
            game._clockWise              = !game._clockWise;
            break;
        case UnoRule::Value::WILD_DRAW_FOUR:
            message["data"]["new_plus4_stacked"] = true;
            game._plus4Stacked = true;
            break;
    }

    if (cardColor == UnoRule::Color::WILD)
    {
        // Modify rule buttons to change color, update message, then send
        int i = 0;
        for (auto& button : game._colorButtons)
        {
            button->setVisible(true);
            static_cast<ax::ui::Button*>(button)->setTitleText(magic_enum::enum_name(static_cast<UnoRule::Color>(i)));
            game._colorButtons.at(i)->addClickEventListener([this, &game, i](ax::Object* sender) {
                game._currentColor = static_cast<UnoRule::Color>(i);
                message["data"]["new_current_color"] = game._currentColor;
                setNewCurrentPlayer();
                game._socketManager->sendMessage(message);
                for (auto button : game._colorButtons)
                    button->setVisible(false);
            });
            i++;
        }
        return;
    }

    setNewCurrentPlayer();
    game._socketManager->sendMessage(message);
    game.changeState(new UnoPlayState(getContext()));
    
}

void UnoPlayState::setNewCurrentPlayer() {
    auto& game = *getContext();
    // Change turn
    game._currentPlayerId += (game._clockWise ? 1 : -1);
    if (game._currentPlayerId >= game._playerCount)
        game._currentPlayerId = 0;
    else if (game._currentPlayerId < 0)
        game._currentPlayerId = game._playerCount - 1;
    message["data"]["new_current_player"] = game._currentPlayerId;
}
