#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/object/Zone.h"

#include <map>
#include <vector>

//global game state that everything can access

class GameState{
public:
    std::vector<Card*> cardsInPlay;
    std::vector<Zone*> zonesInPlay;

    int playerCount = 0;
    int currentPlayerIndex = 0;
    std::vector<std::string> playerNames;

};
