#include "ShuffleCommand.h"
#include "utils/random.hpp"
#include "core/scene/GameScene.h"

using Random = lib::random_static;

ShuffleCommand::ShuffleCommand(ax::Vector<Card*> &cards) : _cardsToShuffle(cards) {
    this->scheduleUpdate();
}

void ShuffleCommand::execute()
{
    this->scheduleUpdate();
    setRunning(true);
    auto& gameCards = static_cast<GameScene*>(ax::Director::getInstance()->getRunningScene())->cards;
    std::vector<Card*> cardsVector(gameCards.begin(), gameCards.end());
    Random::shuffle(cardsVector.begin(), cardsVector.end());
    gameCards.clear();
    for (auto card : cardsVector)
        gameCards.pushBack(card);
    this->setDone(true);
}
