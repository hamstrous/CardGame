#include "Turn.h"

Turn::Turn(Phase* mainPhase) : _mainPhase(mainPhase) {
    this->scheduleUpdate();
}

void Turn::startTurn()
{
    _mainPhase->startPhase();
}
