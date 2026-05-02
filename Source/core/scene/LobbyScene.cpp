#include "LobbyScene.h"

#include <format>
#include "utils/json.hpp"

using namespace ax;
using namespace ax::ui;
using namespace std;
using json = lib::json;

bool LobbyScene::init()
{
    // super init first
    if (!Scene::init())
    {
        return false;
    }

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(LobbyScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(LobbyScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(LobbyScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(LobbyScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(LobbyScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    scheduleUpdate();
    return true;
}

bool LobbyScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool LobbyScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool LobbyScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void LobbyScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {}

void LobbyScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void LobbyScene::startSocket(string authToken) {}

LobbyScene::~LobbyScene() {}
