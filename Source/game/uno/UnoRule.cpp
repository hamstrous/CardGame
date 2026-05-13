#include "UnoRule.h"
#include "UnoSetUpState.h"

bool UnoRule::init()
{
    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(UnoRule::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(UnoRule::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(UnoRule::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(UnoRule::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(UnoRule::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    _websocketListener                     = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(UnoRule::onWebSocketMessage, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 1);

    _socketManager = SocketNetworkManager::getInstance();

    scheduleUpdate();

    changeState(new UnoSetUpState(this));

    return true;
}
