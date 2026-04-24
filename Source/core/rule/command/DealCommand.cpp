#include "DealCommand.h"
#include "core/model/StateManager.h"

void DealCommand::execute() {
    this->scheduleUpdate();

    auto cards     = StateManager::getInstance()->getGameState()->cards;

    setRunning(true);
    float delay = 0.0f;
    int currentZoneIndex = 0;
    for (int i = 0; i < cards.size(); ++i)
    {
        Card* card = cards.at(i);
        card->moveToZone(_targetZones.at(currentZoneIndex), delay += 0.5f);
        currentZoneIndex = (currentZoneIndex + 1) % 2; // Temp: 2 player's zone at index 0, 1
    }
    auto delayAction = ax::DelayTime::create(delay);
    auto callback = ax::CallFunc::create([this]() {
        this->setDone(true);
        });
    auto sequence    = ax::Sequence::create(delayAction, callback, nullptr);
    this->runAction(sequence);
}
