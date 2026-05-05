#pragma once

#include "axmol.h"

#include "core/model/GameState.h"

//global game state that everything can access

class StateManager{
public:
    static StateManager* getInstance()
    {
        if (!_instance)
        {
            _instance = new StateManager();
        }
        return _instance;
    }
    
    void setGameState(GameState* gameState){
        if (_gameState)
        {
            AXLOGD("GameState already set, ignoring new GameState");
            return;
        }
        _gameState = gameState;
    }
    GameState* getGameState() {
        if (!_gameState)
        {
            _gameState = new GameState();
            AXLOGD("GameState not set yet, returning new instance");
        }
        return _gameState;
    }

private:
    StateManager() = default;  // Private constructor to prevent instantiation
    static StateManager* _instance;

    GameState* _gameState = nullptr;
};
