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
    
    void setGameState(GameState* gameState){ _gameState = gameState; }
    GameState* getGameState() const { return _gameState; }

private:
    StateManager() = default;  // Private constructor to prevent instantiation
    static StateManager* _instance;

    GameState* _gameState = nullptr;
};

StateManager* StateManager::_instance = nullptr;
