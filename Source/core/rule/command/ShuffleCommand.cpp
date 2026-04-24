#include "ShuffleCommand.h"
#include "utils/random.hpp"
#include "core/scene/GameScene.h"
#include "core/model/StateManager.h"

using Random = lib::random_static;

ShuffleCommand::ShuffleCommand(ax::Vector<Card*> &cards) : _cardsToShuffle(cards) {
    this->scheduleUpdate();
}

void ShuffleCommand::execute()
{
    this->scheduleUpdate();
    setRunning(true);
    auto& gameCards = StateManager::getInstance()->getGameState()->cards;
    std::vector<Card*> cardsVector(gameCards.begin(), gameCards.end());
    Random::shuffle(cardsVector.begin(), cardsVector.end());
    gameCards.clear();
    for (auto card : cardsVector)
        gameCards.pushBack(card);
    this->setDone(true);
}
