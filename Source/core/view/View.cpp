#include "View.h"
#include "core/model/StateManager.h"

void View::setUpObjectsForScene() {
    auto director  = ax::Director::getInstance();
    auto scene = director->getRunningScene();
    auto gameState = StateManager::getInstance()->getGameState();

    ax::Vec2 visibleSize = director->getVisibleSize();
    ax::Vec2 origin      = director->getVisibleOrigin();
    ax::Rect safeArea    = director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    if (gameState->clientPlayer->getIndex() == 0)
    {

    }
    else
    {
        auto& zones = gameState->zones;
        zones[0]->setPosition(ax::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 300));
        zones[1]->setPosition(ax::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 300));
    }


}
