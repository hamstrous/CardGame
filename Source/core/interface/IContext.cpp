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
            _previousState = _currentState;
        }
        _currentState = _nextState;
        AX_SAFE_DELETE(_nextState);
        _currentState->onEnter();
    }
}
