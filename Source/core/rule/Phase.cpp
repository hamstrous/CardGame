#include "Phase.h"

Phase::Phase(std::vector<Command*>& commandList) : _commandList(commandList)
{
    ax::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

Phase::~Phase() {
    ax::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void Phase::startPhase() {
    executeCommand();
}

void Phase::executeCommand() {
    if (_commandList[_currentCommandIndex] && !_commandList[_currentCommandIndex]->isExecuted() && !_commandList[_currentCommandIndex]->isExecuting())
    {
        _commandList[_currentCommandIndex]->execute();
    }
}

void Phase::update(float delta) {
    if (_currentCommandIndex < _commandList.size() && _commandList[_currentCommandIndex]->isExecuted())
    {
        _currentCommandIndex++;
    }
    if (_currentCommandIndex >= _commandList.size())
    {
        _isDone = true;
    }
    else
    {
        executeCommand();
    }
}
