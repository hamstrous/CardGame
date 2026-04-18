#include "Phase.h"

Phase::Phase(std::vector<Command*>& commandList, bool isRepeating) : _commandList(commandList), _isRepeating(isRepeating)
{
}

Phase::~Phase() {}

void Phase::startPhase() {
    executeCommand();
    this->scheduleUpdate();
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
    else if (_currentCommandIndex < _commandList.size() && !_commandList[_currentCommandIndex]->isExecuted())
    {
        executeCommand();

    }
    else if (_currentCommandIndex >= _commandList.size() && !_isRepeating)
    {
        _isDone = true;
    }
    else
    {
        _currentCommandIndex = 0;
        for (auto command : _commandList)
        {
            command->setExecuted(false);
            command->setExecuting(false);
        }
    }
}
