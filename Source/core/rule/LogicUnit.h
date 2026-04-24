#pragma once

#include "axmol.h"
#include "Command.h"

class LogicUnit : public ax::Node
{
public:
    LogicUnit(Command* command, LogicUnit* next);
    ~LogicUnit();
    LogicUnit* getCurrentLogicUnit() { return _currentLogicUnit; }

    void start();
    void run();
    void end();
    void update(float delta) override;
    
    void setCommand(Command* command) { _command = command; }
    void setNext(LogicUnit* next) { _next = next; }

    void setConditionList(const std::vector<std::function<bool()>>& conditions) { _conditionList = conditions; }
    void conditionsCheckAndStart();

    void setDone(bool isDone) { _isDone = isDone; }
    bool isDone() const { return _isDone; }
    void setRunning(bool isRunning) { _isRunning = isRunning; }
    bool isRunning() const { return _isRunning; }

    void setIsConditional(bool isConditional) { _isConditional = isConditional; }
    bool isConditional() const { return _isConditional; }
    void setIsSelfLoop(bool isSelfLoop) { _isSelfLoop = isSelfLoop; }
    bool isSelfLoop() const { return _isSelfLoop; }
    void setIsAutoReset(bool isAutoReset) { _isAutoReset = isAutoReset; }
    bool isAutoReset() const { return _isAutoReset; }

    void reset();
    void resetChain();

    void startNext();

protected:
    static LogicUnit* _currentLogicUnit;  // Static pointer to keep track of the currently running logic unit

    Command* _command = nullptr;
    
    LogicUnit* _next = nullptr;
    LogicUnit* _previous = nullptr;
    
    std::vector<std::function<bool()>> _conditionList = std::vector<std::function<bool()>>();  // For conditional logic units, store the conditions to check

    bool _isRunning = false;
    bool _isDone = false;

    bool _isConditional = false;  // Can be jump to if conditions are met
    bool _isSelfLoop = false;
    bool _isAutoReset   = false;
};
