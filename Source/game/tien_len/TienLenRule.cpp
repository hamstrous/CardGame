#include "TienLenRule.h"
#include "TienLenSetUpState.h"
#include "TienLenResultState.h"
#include "TienLenPlayState.h"

#include "core/scene/GameScene.h"

#include "utils/magic_enum.hpp"

bool TienLenRule::init()
{
    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(TienLenRule::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(TienLenRule::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(TienLenRule::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(TienLenRule::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(TienLenRule::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    _websocketListener                     = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(TienLenRule::onWebSocketMessage, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 1);

    _cardListener = EventListenerCard::create();
    _cardListener->onCardClicked = AX_CALLBACK_1(TienLenRule::onCardClicked, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_cardListener, this);

    _socketManager = SocketNetworkManager::getInstance();

    _gameInfo = ax::ui::Text::create("Name: \nOrder: \nCurrent turn: \nLast played card: ", "Arial", 20);
    _gameInfo->setPositionX(origin.x + visibleSize.width - _gameInfo->getContentSize().width);
    _gameInfo->setPositionY(origin.y + _gameInfo->getContentSize().height/2);
    this->addChild(_gameInfo);

    scheduleUpdate();

    changeState(new TienLenSetUpState(this));

    return true;
}

void TienLenRule::update(float delta) {
    
    if (dynamic_cast<TienLenPlayState*>(_currentState))
    {
        // update game info
        std::string orderText    = "Order: " + std::string(_clockWise ? "Clockwise" : "Counter-clockwise");
        std::string turnText     = "Current turn: Player " + std::to_string(_currentPlayerId);
        std::string lastCardText = std::string("Last played card: ") +
                                   std::string(magic_enum::enum_name(_currentValue)) + "-" +
                                   std::string(magic_enum::enum_name(_currentColor));
        _gameInfo->setString(std::string("Name: ") + GameScene::getInstance()->getUserName() + "\n" + orderText + "\n" +
                             turnText + "\n" + lastCardText);

        for (int i = 0; i < _playerHands.size(); i++)
        {
            auto zone        = _playerHands[i];
            auto cardsInHand = helper::castToVectorOfType<Card*>(zone->getChildren());
            if (cardsInHand.empty())
            {
                if (i == _clientPlayerId)
                {
                    changeState(new TienLenResultState(this, "You win!"));
                }
                else
                {
                    changeState(new TienLenResultState(this, "You lose!"));
                }
                break;
            }
        }
    }
    Rule::update(delta);
}

void TienLenRule::onCardClicked(EventCard* event) {
    if (_currentState)
    {
        static_cast<TienLenState*>(_currentState)->onCardClicked(event);
    }
}
