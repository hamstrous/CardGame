#pragma once

#include "axmol.h"
#include "Command.h"

class Phase : public ax::Node
{
    enum class State {
        prepare,
        main,
        done
    };

public:
    Phase(std::vector<Command*> &commandList);
    ~Phase();
    virtual void startPhase();
    void executeCommand();
    void update(float delta) override;
    bool isDone() const { return _isDone; }

protected:
    //std::vector<Command*> _commandList;
    std::vector<Command*> _commandList;
    int _currentCommandIndex = 0;
    State _currentState = State::prepare;
    bool _isDone             = false;
};
