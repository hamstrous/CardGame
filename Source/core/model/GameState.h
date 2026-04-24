#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/object/Zone.h"

#include <map>
#include <vector>

//global game state that everything can access

class GameState{
public:
    static GameState* getInstance()
    {
        if (!_instance)
        {
            _instance = new GameState();
        }
        return _instance;
    }

    std::vector<Card*> cardsInPlay;
    std::vector<Zone*> zonesInPlay;

    int playerCount = 0;
    int currentPlayerIndex = 0;
    std::vector<std::string> playerNames;

private:
    static GameState* _instance;
};

GameState* GameState::_instance = nullptr;
