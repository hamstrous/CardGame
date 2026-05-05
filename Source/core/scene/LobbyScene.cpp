#include "LobbyScene.h"

#include "core/model/StateManager.h"

#include "core/event/EventWebSocket.h"
#include "core/network/SocketNetworkManager.h"

#include "ui/UIButton.h"

#include <format>
#include "utils/json.hpp"

#include "core/scene/MenuScene.h"

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

    _websocketListener                     = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(LobbyScene::onWebSocketMessage, this);
    _websocketListener->onWebSocketOpen    = [](EventWebSocket* event) {};
    _websocketListener->onWebSocketError   = [](EventWebSocket* event) {};
    _websocketListener->onWebSocketClose   = [](EventWebSocket* event) {};
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 11);
    scheduleUpdate();

    _roomIdText =
        Label::createWithSystemFont("Room ID: " + StateManager::getInstance()->getGameState()->roomId, "Arial", 24);
    _roomIdText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
    _roomIdText->setTextColor(Color4B::WHITE);
    this->addChild(_roomIdText);

    SocketNetworkManager::getInstance()->sendMessage(json{{"type", "request"},
                                                          {"command", "list_users_in_room"},
                                                          {"data", json::object()},
                                                          {"id", 0},
                                                          {"time_stamp", 0}});

    auto _joinGameButton = Button::create("background.png");
    _joinGameButton->ignoreContentAdaptWithSize(false);
    _joinGameButton->setContentSize(Size(150, 50));
    _joinGameButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
    _joinGameButton->setTitleText("Join Game");
    _joinGameButton->addClickEventListener([this](ax::Object* sender) {
        _director->replaceScene(StateManager::getInstance()->getGameState()->gameScene);
    });
    this->addChild(_joinGameButton);

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

void LobbyScene::onWebSocketMessage(EventWebSocket* event)
{
    json message   = event->getData();
    string command = message["command"];
    if (command == "list_users_in_room")
    {
        vector<string> users = message["data"]["user_list"].get<vector<string>>();
        _usersInRoom.clear();
        for (size_t i = 0; i < users.size(); ++i)
        {
            auto userText = Label::createWithSystemFont(users[i], "Arial", 24);
            userText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50 - i * 30));
            userText->setTextColor(Color4B::WHITE);
            this->addChild(userText);
            _usersInRoom.pushBack(userText);
        }
    }
    else if (command == "user_joined")
    {
        string user   = message["data"]["username"];
        auto userText = Label::createWithSystemFont(user, "Arial", 24);
        userText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50 - _usersInRoom.size() * 30));
        userText->setTextColor(Color4B::WHITE);
        this->addChild(userText);
        _usersInRoom.pushBack(userText);
    }
    else if (command == "user_left")
    {
        string user = message["data"]["user"];
        for (auto& text : _usersInRoom)
        {
            if (text->getString() == user)
            {
                text->removeFromParent();
                _usersInRoom.eraseObject(text);
                break;
            }
        }
    }
    else
    {
        AXLOGD("Received unknown WebSocket message command: {}", command);
    }
}

void LobbyScene::onEnter()
{
    Scene::onEnter();
    // _director->replaceScene(utils::createInstance<MenuScene>());
}

void LobbyScene::startSocket(string authToken) {}

LobbyScene::~LobbyScene() {}
