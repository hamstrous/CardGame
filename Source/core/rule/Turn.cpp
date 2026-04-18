#include "Turn.h"

Turn::Turn(Phase* mainPhase) : _mainPhase(mainPhase) {
    this->scheduleUpdate();
    ax::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void Turn::startTurn()
{
    _mainPhase->startPhase();
}
