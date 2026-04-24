#include "LogicUnit.h"

LogicUnit* LogicUnit::_currentLogicUnit = nullptr;  // pointer to singleton

LogicUnit::LogicUnit(Command* command, LogicUnit* next) : _command(command), _next(next) {
    this->addChild(_command);
    _command->setOnCompleteCallback([this]() {
        this->setDone(true);
    });
    scheduleUpdate();
}

LogicUnit::~LogicUnit() {}

void LogicUnit::start() {
    _currentLogicUnit = this;
    _command->execute();
    setRunning(true);
}

void LogicUnit::run() {

}

void LogicUnit::update(float delta) {
    conditionsCheckAndStart();
    if (!isRunning())
        return;
    if (isDone())
        startNext();
}

void LogicUnit::conditionsCheckAndStart() {
    if (_conditionList.empty())
        return;
    
    bool allConditionsMet = true;
    for (const auto& condition : _conditionList)
    {
        if (!condition())
        {
            allConditionsMet = false;
            break;
        }
    }
    if (allConditionsMet)
    {
        _previous = LogicUnit::getCurrentLogicUnit();
        start();
    }
}

void LogicUnit::reset() {
    _isRunning = false;
    _isDone    = false;
    if (_command)
    {
        _command->setDone(false);
        _command->setRunning(false);
    }
}

void LogicUnit::resetChain()
{
    reset();
    if (_next)
    {
        _next->resetChain();
    }
}

void LogicUnit::startNext() {
    if (_isDone && _next)
    {
        _next->_previous = this;
        _isRunning       = false;
        if (isAutoReset())
        {
            reset();
        }
        _next->start();
    }
}
