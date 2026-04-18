#include "Phase.h"

Phase::Phase(Command* prepareCommand, Command* mainCommand) : _prepareCommand(prepareCommand), _mainCommand(mainCommand) {
    this->scheduleUpdate();
}

Phase::~Phase() {
    ax::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void Phase::startPhase() {
    _prepareCommand->execute();
}

void Phase::update(float delta) {

}
