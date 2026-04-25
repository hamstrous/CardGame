#include "MainGameCommand.h"
#include "core/scene/GameScene.h"
#include "core/model/StateManager.h"
#include "core/network/HttpRequestHandler.h"

MainGameCommand::MainGameCommand(Zone* playField) 
{
    _playField = playField;
    _zoneListener = EventListenerZone::create();
    _zoneListener->onCardReceived = AX_CALLBACK_1(MainGameCommand::onMainFieldCardReceived, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_zoneListener, _playField);
}

void MainGameCommand::execute()
{
    auto gameScene = static_cast<GameScene*>(ax::Director::getInstance()->getRunningScene());
    auto cards     = StateManager::getInstance()->getGameState()->cards;
    auto zones     = StateManager::getInstance()->getGameState()->zones;
    if (_firstTime)
    {
        for (auto i : cards)
        {
            if (i->getParent() == zones[0])
                _playerList[0].push_back(i);
            else if (i->getParent() == zones[1])
                _playerList[1].push_back(i);
        }
        _firstTime = false;
        _playerList[0].push_back(zones[0]);
        _playerList[1].push_back(zones[1]);
        this->scheduleUpdate();
        setRunning(true);
    }
    setCurrentPlayerIndex(1 - _currentPlayerIndex);
}

void MainGameCommand::update(float delta) {}

void MainGameCommand::onMainFieldCardReceived(EventZone* event) {
    if (event->getZone() != _playField) {
        return;  // Ignore events from other zones
    }

    //setDone(true);
    for (auto input : _playerList[_currentPlayerIndex])
    {
        input->lockInput();
    }

    Card* card = event->getCard();

    _playerList[_currentPlayerIndex].erase(std::remove(_playerList[_currentPlayerIndex].begin(), _playerList[_currentPlayerIndex].end(), card), _playerList[_currentPlayerIndex].end());


    HttpRequestHandler::sendPostRequest("http://localhost:5284/play/" + std::to_string(_currentPlayerIndex) + "/" + std::to_string(card->getId()),
        [this](HttpClient* client, HttpResponse* response) {
            if (response->getResponseCode() == 200)
            {
                AXLOG("Play successful");
            }
            else
            {
                AXLOG("Play failed, response code: %d", response->getResponseCode());
            }
        }
    );

    if (_playerList[_currentPlayerIndex].size() == 1)
    {
        AXLOG("%d win", _currentPlayerIndex);
        setDone(true);
        return;
    }
    setCurrentPlayerIndex(1 - _currentPlayerIndex);
}

void MainGameCommand::setCurrentPlayerIndex(int index) {
    _currentPlayerIndex = index;
    auto gameState      = StateManager::getInstance()->getGameState();

    // if it's the client's turn, unlock their inputs; otherwise, keep them locked
    if (gameState->clientPlayer->getIndex() == _currentPlayerIndex)
    {
        for (auto input : _playerList[_currentPlayerIndex])
        {
            input->unlockInput();
        }   
    }
    else
    {
        auto callable = [this](HttpClient* client, HttpResponse* response) {
            if (response->getResponseCode() == 200)
            {
                int previousPlayedCardIndex = HttpRequestHandler::convertBufferToInt(response->getResponseData());
                auto cards                  = StateManager::getInstance()->getGameState()->cards;
                for (auto card : cards)
                    if (card->getId() == previousPlayedCardIndex)
                        card->moveToZone(_playField);
                AXLOG("Checked turn successfully, current player index: %d", _currentPlayerIndex);
                for (auto input : _playerList[_currentPlayerIndex])
                {
                    input->lockInput();
                }
                if (_playerList[_currentPlayerIndex].size() == 1)
                {
                    AXLOG("%d win", _currentPlayerIndex);
                    setDone(true);
                    return;
                }
                setCurrentPlayerIndex(1 - _currentPlayerIndex);

            }
            else
            {
                AXLOG("Opponent hasn't played yet, retrying in 5s... response code: %d", response->getResponseCode());

                this->scheduleOnce([this](float dt) {
                    HttpRequestHandler::sendGetRequest(
                        "http://localhost:5284/play/" +
                            std::to_string(StateManager::getInstance()->getGameState()->clientPlayer->getIndex()),
                        [this](HttpClient* client, HttpResponse* response) {
                            setCurrentPlayerIndex(_currentPlayerIndex);
                        });
                }, 5.0f, "retry_check_turn");
            }
        };
        
        HttpRequestHandler::sendGetRequest(
            "http://localhost:5284/play/" + std::to_string(gameState->clientPlayer->getIndex()), callable);
        
    }
}
