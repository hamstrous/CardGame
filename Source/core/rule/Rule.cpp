#include "Rule.h"

#include "GameState.h"

bool Rule::init()
{
    _mouseListener = ax::EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(Rule::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(Rule::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(Rule::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener = ax::EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = AX_CALLBACK_2(Rule::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(Rule::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    _websocketListener = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(Rule::onWebSocketMessage, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_websocketListener, this);

    return true;
}

bool Rule::onlineInit()
{
    return false;
}


Rule::Rule() {

}

void Rule::update(float delta) {
    if (_currentState)
    {
        _currentState->onUpdate(delta);
    }
    handleStateChange();
}

bool Rule::onMouseDown(ax::Event* event)
{
    if (_currentState)
    {
        getCurrentState()->onMouseDown(event);
    }
    return true;
}

bool Rule::onMouseUp(ax::Event* event)
{
    if (_currentState)
    {
        getCurrentState()->onMouseUp(event);
    }
    return true;
}

bool Rule::onMouseMove(ax::Event* event)
{
    if (_currentState)
    {
        getCurrentState()->onMouseMove(event);
    }
    return true;
}

bool Rule::onMouseScroll(ax::Event* event)
{
    if (_currentState)
    {
        getCurrentState()->onMouseScroll(event);
    }
    return true;
}

void Rule::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event)
{
    if (_currentState)
    {
        getCurrentState()->onKeyPressed(code, event);
    }
}

void Rule::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event)
{
    if (_currentState)
    {
        getCurrentState()->onKeyReleased(code, event);
    }
}

void Rule::onWebSocketMessage(EventWebSocket* event)
{
    if (_currentState)
    {
        getCurrentState()->onWebSocketMessage(event);
    }
}


void Rule::setPlayerId(int playerId)
{
    _clientPlayerId = playerId;
    if (_clientPlayerId == 0)
        _isHost = true;
}

void Rule::setPlayerCount(int playerCount) {
    _playerCount = playerCount;
}

void Rule::setUserName(const std::string& userName) {
    _userName = userName;
}

GameState* Rule::getCurrentState()
{
    return static_cast<GameState*>(_currentState);
}
