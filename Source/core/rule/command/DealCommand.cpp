#include "DealCommand.h"

void DealCommand::execute() {
    float delay = 0.0f;
    int currentZoneIndex = 0;
    for (int i = 0; i < _cardsToDeal.size(); ++i)
    {
        Card* card = _cardsToDeal.at(i);
        card->moveToZone(_targetZones.at(currentZoneIndex), delay += 0.5f);
        currentZoneIndex = (currentZoneIndex + 1) % _targetZones.size();
    }
    auto delayAction = ax::DelayTime::create(delay);
    auto callback = ax::CallFunc::create([this]() {
        this->setExecuted(true);
        AXLOG("Deal command done");
        });
    auto sequence    = ax::Sequence::create(delayAction, callback, nullptr);
    this->runAction(sequence);
    //ax::Director::getInstance()->getRunningScene()->runAction(sequence);
}
