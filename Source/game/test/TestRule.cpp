#include "TestRule.h"

#include "core/scene/GameScene.h"
#include "TestState.h"

#include "utils/magic_enum.hpp"

bool TestRule::init()
{
    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(TestRule::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(TestRule::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(TestRule::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(TestRule::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(TestRule::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    _websocketListener                     = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(TestRule::onWebSocketMessage, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 1);

    _socketManager = SocketNetworkManager::getInstance();

    scheduleUpdate();

    changeState(new TestState(this));

    return true;
}

void TestRule::update(float delta) {
 
    Rule::update(delta);
}
