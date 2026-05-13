#pragma once

#include "axmol.h"

#include "core/event/EventListenerWebsocket.h"
#include "core/event/EventListenerCard.h"
#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/object/Deck.h"

#include "core/rule/Rule.h"



class UnoRule : public Rule
{
public:
    enum class Color
    {
        RED,
        YELLOW,
        GREEN,
        BLUE,
        WILD
    };

    enum class Value
    {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        SKIP,
        REVERSE,
        DRAW_TWO,
        WILD,
        WILD_DRAW_FOUR
    };

    enum class Phase
    {
        WAIT,
        SHUFFLE,
        DEAL,
        PLAY,
        RESULT
    };

public:

    // WebSocket
    void onWebSocketMessage(EventWebSocket* event) override;
    void onAllPlayersReady(EventWebSocket* event);
    void onShuffleComplete(EventWebSocket* event);
    void onDealComplete(EventWebSocket* event);
    void onCardPlayed(EventWebSocket* event);

    // Player actions
    //void onPlayCard(EventCard* event);

protected:

    bool _clockWise                            = true;
    EventListenerWebSocket* _websocketListener = nullptr;
    // state
    ax::Vector<Card*> _cards;
    ax::Vector<Zone*> _playerHands;  // represent player's hand
    Zone* _discardPile = nullptr;    // represent the discard pile
    Deck* _deck        = nullptr;    // represent the deck

    EventListenerCard* _cardEventListener = nullptr;

    Color _currentColor = Color::RED;
    Value _currentValue = Value::ZERO;

    Phase _currentPhase = Phase::WAIT;
};
