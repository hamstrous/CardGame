#include "GameScene.h"
#include "core/network/HttpRequestHandler.h"


using namespace ax;
using namespace ax::network;
using namespace std;

GameScene* GameScene::_instance = nullptr;

GameScene* GameScene::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new GameScene();
        if (!_instance->init())
        {
            AX_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

bool GameScene::init()
{
    // super init first
    if (!Scene::init())
    {
        return false;
    }

    AXLOGD("GameScene init");

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(GameScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(GameScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(GameScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    scheduleUpdate();

    return true;
}

void GameScene::update(float delta) {}

bool GameScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool GameScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool GameScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void GameScene::setUserName(std::string userName) {
    _userName = userName;
}

std::string GameScene::getUserName() {
    return _userName;
}

void GameScene::setRoomId(std::string roomId)
{
    _roomId = roomId;
}

std::string GameScene::getRoomId()
{
    return _roomId;
}

void GameScene::setPlayerCount(int playerCount) {
    _playerCount = playerCount;
}

int GameScene::getPlayerCount()
{
    return _playerCount;
}

void GameScene::setPlayerId(int playerId) {
    _playerId = playerId;
}

int GameScene::getPlayerId()
{
    return _playerId;
}

void GameScene::onEnter()
{
    Scene::onEnter();
    AXLOGD("GameScene onEnter");
}

GameScene::~GameScene() {}
