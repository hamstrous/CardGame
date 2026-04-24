#include "MainGameCommand.h"
#include "core/scene/GameScene.h"

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
    auto cards     = gameScene->cards;
    auto zones     = gameScene->zones;
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
    }
   
    this->scheduleUpdate();
    setRunning(true);
    setCurrentPlayerIndex(1 - _currentPlayerIndex);
}

void MainGameCommand::update(float delta) {}

void MainGameCommand::onMainFieldCardReceived(EventZone* event) {
    if (event->getZone() != _playField) {
        return;  // Ignore events from other zones
    }

    setDone(true);
    for (auto input : _playerList[_currentPlayerIndex])
    {
        input->lockInput();
    }

    Card* card = event->getCard();

    _playerList[_currentPlayerIndex].erase(std::remove(_playerList[_currentPlayerIndex].begin(), _playerList[_currentPlayerIndex].end(), card), _playerList[_currentPlayerIndex].end());
    if (_playerList[_currentPlayerIndex].size() == 1)
    {
        AXLOG("%d win", _currentPlayerIndex);
    }

}

void MainGameCommand::setCurrentPlayerIndex(int index) {
    _currentPlayerIndex = index;
    for (auto input : _playerList[_currentPlayerIndex])
    {
        input->unlockInput();
    }
}
