#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/object/Zone.h"

#include "core/view/Player.h"

#include <map>
#include <vector>

//global game state that everything can access

class GameState{
public:
    GameState() = default;
    // global
    ax::Vector<Card*> cards;
    ax::Vector<Zone*> zones;

    int playerCount = 0;
    int currentPlayerIndex = 0;
    std::vector<std::string> playerNames;

    std::string roomId = "";

    // local
    Player* clientPlayer = nullptr;

};
