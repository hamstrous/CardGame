#include "ShuffleCommand.h"
#include "utils/random.hpp"
#include "core/scene/GameScene.h"
#include "core/model/StateManager.h"

#include "core/network/HttpRequestHandler.h"

using Random = lib::random_static;

ShuffleCommand::ShuffleCommand(ax::Vector<Card*> &cards) : _cardsToShuffle(cards) {
    this->scheduleUpdate();
}

void ShuffleCommand::execute()
{
    this->scheduleUpdate();
    setRunning(true);
    auto& gameCards = StateManager::getInstance()->getGameState()->cards;

    HttpRequestHandler::sendGetRequest("http://localhost:5284/shuffle/" + std::to_string(gameCards.size()),
                                                            [this, &gameCards](HttpClient* client, HttpResponse* response) {
        if (response->getResponseCode() == 200)
        {
            std::vector<int> shuffledIndices = HttpRequestHandler::convertBufferToVectorOfInt(response->getResponseData());
            std::vector<Card*> shuffledCards(gameCards.size());
            for (size_t i = 0; i < shuffledIndices.size(); ++i)
                shuffledCards[i] = gameCards.at(shuffledIndices[i]);
            gameCards.clear();
            for (auto card : shuffledCards)
                gameCards.pushBack(card);
        }
        else
        {
            std::vector<Card*> cardsVector(gameCards.begin(), gameCards.end());
            Random::shuffle(cardsVector.begin(), cardsVector.end());
            gameCards.clear();
            for (auto card : cardsVector)
                gameCards.pushBack(card);
        }
        this->setDone(true);
    });
}
