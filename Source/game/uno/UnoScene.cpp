#include "UnoScene.h"

#include "core/network/HttpRequestHandler.h"

#include "utils/helper.h"

using namespace ax;
using namespace ax::network;
using namespace std;
using namespace helper;

bool UnoScene::init()
{
    // super init first
    if (!Scene::init())
    {
        return false;
    }

   
}

void UnoScene::update(float delta) {}


bool UnoScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool UnoScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool UnoScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void UnoScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {}

void UnoScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void UnoScene::onWebSocketMessage(EventWebSocket* event) {}

void UnoScene::onAllPlayersReady(EventWebSocket* event) {}

void UnoScene::onCardPlayed(EventWebSocket* event) {}

void UnoScene::onPlayCard(EventCard* event) {}

void UnoScene::onEnter() {
    Scene::onEnter();
}

UnoScene::~UnoScene() {}
