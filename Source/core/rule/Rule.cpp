#include "Rule.h"

#include "core/rule/GameState.h"

bool Rule::init()
{

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
        handleStateChange();
    }
}

bool Rule::onMouseDown(ax::Event* event)
{
    if (_currentState)
    {
       static_cast<GameState*>(_currentState)->onMouseDown(event);
    }
    return true;
}

bool Rule::onMouseUp(ax::Event* event)
{
    if (_currentState)
    {
        static_cast<GameState*>(_currentState)->onMouseUp(event);
    }
    return true;
}

bool Rule::onMouseMove(ax::Event* event)
{
    if (_currentState)
    {
        static_cast<GameState*>(_currentState)->onMouseMove(event);
    }
    return true;
}

bool Rule::onMouseScroll(ax::Event* event)
{
    if (_currentState)
    {
        static_cast<GameState*>(_currentState)->onMouseScroll(event);
    }
    return true;
}

void Rule::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event)
{
    if (_currentState)
    {
        static_cast<GameState*>(_currentState)->onKeyPressed(code, event);
    }
}

void Rule::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event)
{
    if (_currentState)
    {
        static_cast<GameState*>(_currentState)->onKeyReleased(code, event);
    }
}

void Rule::onWebSocketMessage(EventWebSocket* event)
{
    if (_currentState)
    {
        static_cast<GameState*>(_currentState)->onWebSocketMessage(event);
    }
}


void Rule::setPlayerId(int playerId)
{
    _playerId = playerId;
}

void Rule::setPlayerCount(int playerCount) {
    _playerCount = playerCount;
}

void Rule::setUserName(const std::string& userName) {
    _userName = userName;
}
