#pragma once

#include "axmol.h"

class IState;

class IContext
{
public:
    virtual ~IContext() {}

    // State management
    void changeState(IState* newState);

    void handleStateChange();

    virtual IState* getCurrentState() const { return _currentState; }

protected:
    IState* _currentState  = nullptr;
    IState* _previousState = nullptr;
    IState* _nextState     = nullptr;
};
