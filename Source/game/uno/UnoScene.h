#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/object/Deck.h"
#include "core/event/EventListenerZone.h"
#include "core/event/EventListenerWebSocket.h"
#include "core/network/SocketNetworkManager.h"

#include "core/rule/Rule.h"
#include "core/rule/LogicUnit.h"
#include "core/model/GameState.h"


class UnoScene : public ax::Scene
{
public:
    enum class Phase
    {
        WAIT,
        SHUFFLE,
        DEAL,
        PLAY,
        RESULT
    };

public:

    bool init() override;
    void update(float delta) override;

    void setUpObjects();

    // mouse
    bool onMouseDown(ax::Event* event);
    bool onMouseUp(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseScroll(ax::Event* event);

    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // WebSocket
    void onWebSocketMessage(EventWebSocket* event);
    void onAllPlayersReady(EventWebSocket* event);
    void onShuffleComplete(EventWebSocket* event);
    void onDealComplete(EventWebSocket* event);
    void onCardPlayed(EventWebSocket* event);

    // Player actions
    void onPlayCard(EventCard* event);


    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    void onEnter() override;

    ~UnoScene() override;

protected:
    // online
    SocketNetworkManager* _socketManager = nullptr;

    bool _isHost = false;
    int _playerId = -1;
    int _playerCount = 0;
    EventListenerWebSocket* _websocketListener = nullptr;
    // state
    ax::Vector<Card*> _cards;
    ax::Vector<Zone*> _playerHands;  // represent player's hand
    Zone* _discardPile = nullptr;    // represent the discard pile
    Deck* _deck        = nullptr;    // represent the deck

    EventListenerCard* _cardEventListener = nullptr;

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

    Color _currentColor = Color::RED;
    Value _currentValue = Value::ZERO;

    Phase _currentPhase = Phase::WAIT;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    int _sceneID                                 = 0;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    GameState* _gameState = nullptr; 

    //EventListenerZone* _cardEventListener = nullptr;
};
