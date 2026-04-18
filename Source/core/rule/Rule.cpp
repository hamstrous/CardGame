#include "Rule.h"

Rule::Rule(Turn* startTurn, Turn* mainTurn, Turn* endTurn) : _startTurn(startTurn), _mainTurn(mainTurn), _endTurn(endTurn) {
    ax::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
    _currentTurn = _startTurn;
}

void Rule::update(float delta) {
    if (_currentTurn->isDone()) {
        switch (_currentState) {
            case State::start:
                _currentTurn = _mainTurn;
                executeTurn();
                _currentState = State::main;
                break;
            case State::main:
                _currentTurn = _endTurn;
                executeTurn();
                _currentState = State::end;
                break;
            case State::end:
                // Rule is done, you might want to reset or do something else here
                break;
        }
    }
}
