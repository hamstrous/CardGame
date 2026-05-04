#include "RoomScene.h"

#include <format>
#include "utils/json.hpp"

#include "core/event/EventListenerWebsocket.h"
#include "core/event/EventWebsocket.h"

#include "core/scene/LobbyScene.h"

using namespace ax;
using namespace ax::ui;
using namespace std;
using json = lib::json;

bool RoomScene::init()
{
    // super init first
    if (!Scene::init())
    {
        return false;
    }

    _socketManager = SocketNetworkManager::getInstance();

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _websocketListener = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(RoomScene::onWebSocketMessage, this);
    _websocketListener->onWebSocketOpen    = [](EventWebSocket* event) {};
    _websocketListener->onWebSocketError   = [](EventWebSocket* event) {};
    _websocketListener->onWebSocketClose   = [](EventWebSocket* event) {};
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 11);


    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(RoomScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(RoomScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(RoomScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(RoomScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(RoomScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    scheduleUpdate();

    _createRoomButton = Button::create("background.png");
    _createRoomButton->ignoreContentAdaptWithSize(false);
    _createRoomButton->setContentSize(Size(100, 50));
    _createRoomButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
    _createRoomButton->setTitleLabel(Label::createWithSystemFont("create room", "Arial", 24));

    _createRoomButton->addClickEventListener([this](ax::Object* sender) {
        json message;
        message["command"] = "create_room";
        message["type"] = "request";
        message["id"] = 0;
        message["data"] = json::object();
        _socketManager->sendMessage(message);
    });

    this->addChild(_createRoomButton);

    _joinRoomButton = Button::create("background.png");
    _joinRoomButton->ignoreContentAdaptWithSize(false);
    _joinRoomButton->setContentSize(Size(100, 50));
    _joinRoomButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 160));
    _joinRoomButton->setTitleLabel(Label::createWithSystemFont("join room", "Arial", 24));
    this->addChild(_joinRoomButton);

    _joinRoomButton->addClickEventListener([this](ax::Object* sender) {
        string roomId = _roomIdEditBox->getText();
        if (roomId.empty())
        {
            AXLOGD("Join room button clicked, but room ID is empty");
            return;
        }
        json message;
        message["command"]  = "join_room";
        message["type"] = "request";
        message["id"]   = 0;
        message["data"] = {{"room_id", roomId}};
        _socketManager->sendMessage(message);
    });

    _roomIdEditBox = EditBox::create(Size(200, 40), "background.png");
    _roomIdEditBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));
    this->addChild(_roomIdEditBox);

    return true;
}

bool RoomScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool RoomScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool RoomScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void RoomScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {}

void RoomScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void RoomScene::onWebSocketMessage(EventWebSocket* event) {
    json data = event->getData();
    string cmd = data["command"];

    if (cmd == "create_room")
    {
        onCreateRoomMessage(event);
    }
    else if (cmd == "join_room")
    {
        onJoinRoomMessage(event);
    }
    else
    {
        AXLOGD("Received unknown WebSocket message: {}", cmd);

    }
}

void RoomScene::onCreateRoomMessage(EventWebSocket* event)
{
    json data = event->getData();

    std::string roomId = data["data"]["room_id"];
    if (roomId.empty())
    {
        AXLOGD("Received create room message, but room ID is missing");
        return;
    }
    AXLOGD("Received create room message, room ID: {}", roomId);
    _director->replaceScene(utils::createInstance<LobbyScene>());
    
}

void RoomScene::onJoinRoomMessage(EventWebSocket* event) {
    json data = event->getData();
    std::string roomId = data["data"]["room_id"];
    if (roomId.empty())
    {
        AXLOGD("Received join room message, but room ID is missing");
        return;
    }
    AXLOGD("Received join room message, room ID: {}", roomId);
    _director->replaceScene(utils::createInstance<LobbyScene>());
}

void RoomScene::startSocket(string authToken) {
    
}

RoomScene::~RoomScene() {}
