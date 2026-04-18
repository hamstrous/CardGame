#include "Rule.h"

Rule::Rule(Turn* startTurn, Turn* mainTurn, Turn* endTurn) : _startTurn(startTurn), _mainTurn(mainTurn), _endTurn(endTurn) {
    this->scheduleUpdate();
    _currentTurn = _startTurn;
}

void Rule::update(float delta) {
    if (_currentTurn->isDone()) {
        switch (_currentState) {
            case State::start:
                _currentTurn = _mainTurn;
                _currentTurn->startTurn();
                _currentState = State::main;
                break;
            case State::main:
                _currentTurn = _endTurn;
                _currentTurn->startTurn();
                _currentState = State::end;
                break;
            case State::end:
                // Rule is done, you might want to reset or do something else here
                break;
        }
    }
}
