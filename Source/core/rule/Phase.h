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
    Phase(Command* prepareCommand, Command* mainCommand);
    ~Phase();
    virtual void startPhase();
    void update(float delta);

protected:
    //std::vector<Command*> _commandList;
    Command* _prepareCommand;
    Command* _mainCommand;

    Command* _currentCommand;
    State _currentState = State::prepare;
};
