#pragma once

#include "axmol.h"
#include "Turn.h"

class Rule : public ax::Node
{
    enum class State {
        start,
        main,
        end
    };

public:
    Rule(Turn* prepareTurn, Turn* mainTurn, Turn* endTurn);
    ~Rule() { ax::Director::getInstance()->getScheduler()->unscheduleUpdate(this); }

    void startRule() {
        startTurn();
    }

    void startTurn() {
        if (_currentTurn) {
            _currentTurn->startTurn();
        }
    }

    void update(float delta);

protected:
    Turn* _startTurn;
    Turn* _mainTurn;
    Turn* _endTurn;
    Turn* _currentTurn = nullptr;

    State _currentState = State::start;

};
