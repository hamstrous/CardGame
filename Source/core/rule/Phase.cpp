#include "Phase.h"

Phase::Phase(std::vector<Command*>& commandList) : _commandList(commandList)
{
    this->scheduleUpdate();
}

Phase::~Phase() {}

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
