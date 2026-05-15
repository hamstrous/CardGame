#include "UnoRule.h"
#include "UnoSetUpState.h"

#include "utils/magic_enum.hpp"

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

    _gameInfo = ax::ui::Text::create("Order: \nCurrent turn: \nLast played card: ", "Arial", 20);
    _gameInfo->setPositionX(origin.x + visibleSize.width - _gameInfo->getContentSize().width);
    _gameInfo->setPositionY(origin.y + _gameInfo->getContentSize().height/2);
    this->addChild(_gameInfo);

    scheduleUpdate();

    changeState(new UnoSetUpState(this));

    return true;
}

void UnoRule::update(float delta) {
    Rule::update(delta);

    // update game info
    std::string orderText = "Order: " + std::string(_clockWise ? "Clockwise" : "Counter-clockwise");
    std::string turnText  = "Current turn: Player " + std::to_string(_currentPlayerId);
    std::string lastCardText = std::string("Last played card: ") + std::string(magic_enum::enum_name(_currentValue)) + "-" + std::string(magic_enum::enum_name(_currentColor));
    _gameInfo->setString(orderText + "\n" + turnText + "\n" + lastCardText);
}
