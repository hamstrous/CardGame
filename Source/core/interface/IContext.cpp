#include "IContext.h"
#include "IState.h"

void IContext::changeState(IState* newState)
{
    if (newState != nullptr && newState != _currentState)
    {
        _nextState = newState;
    }
}

void IContext::handleStateChange()
{
    if (_nextState != nullptr && _nextState != _currentState)
    {
        if (_currentState)
        {
            _currentState->onExit();
            delete _previousState;  // Assuming ownership of the state
            _previousState = _currentState;
        }
        _currentState = _nextState;
        _nextState    = nullptr;
        _currentState->onEnter();
    }
}
