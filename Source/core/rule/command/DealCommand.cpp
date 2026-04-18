#include "DealCommand.h"
#include "core/scene/GameScene.h"

void DealCommand::execute() {
    this->scheduleUpdate();

    auto gameScene = static_cast<GameScene*>(ax::Director::getInstance()->getRunningScene());
    auto cards     = gameScene->cards;

    setExecuting(true);
    AXLOG("Deal command executing");
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
        this->setExecuted(true);
        AXLOG("Deal command done");
        });
    auto sequence    = ax::Sequence::create(delayAction, callback, nullptr);
    this->runAction(sequence);
}
