#pragma once

#include "axmol.h"
#include "Phase.h"

class Turn : public ax::Node
{
public:
    Turn(Phase* mainPhase);
    ~Turn() { ax::Director::getInstance()->getScheduler()->unscheduleUpdate(this); }

    void startTurn();
    bool isDone() const { return _isDone; }
    void update(float delta) {
        
    }

protected:
    bool _isDone = false;
    Phase* _mainPhase;
};
