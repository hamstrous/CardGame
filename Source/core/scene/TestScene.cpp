#include "TestScene.h"

#include "network/HttpClient.h"
#include "core/network/HttpRequestHandler.h"
#include "core/network/SocketNetworkManager.h"

#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIButton.h"

#include "core/scene/RoomScene.h"
#include "core/scene/MenuScene.h"

#include <format>
#include "utils/json.hpp"

using namespace ax;
using namespace ax::network;
using namespace ax::ui;
using namespace std;
using json = nlohmann::json;

bool TestScene::init()
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
    _mouseListener->onMouseMove = AX_CALLBACK_1(TestScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(TestScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(TestScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(TestScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(TestScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    scheduleUpdate();

    //_testImg = Sprite::create("card/uno/0_blue.png");
    //_testImg->


    //this->addChild(_testImg);

    _deck = Deck::create(nullptr);
    _deck->setPosition(visibleSize / 2);
    _deck->setContentSize(ax::Size(300, 150));
    this->addChild(_deck);

    // create 10 cards

    for (int i = 0; i < 10; ++i)
    {
        Card* card = Card::create(new CardData("card/uno/0_" "blue" ".png", "card/Card Back 1.png"));
        card->setPosition(visibleSize / 2 + ax::Vec2(50 + i * 50, 0));
        this->addChild(card);
    }

    // create 4 zones in four sides of the screen
    for (int i = 0; i < 4; ++i)
    {
        Zone* zone = Zone::create(new ZoneData());
        zone->setPosition(visibleSize / 2 + ax::Vec2((i - 1.5f) * 300, (i % 2 == 0 ? -1 : 1) * 200));
        zone->setContentSize(ax::Size(100, 100));
        this->addChild(zone);
        _zones.pushBack(zone);
    }

    return true;
}

bool TestScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool TestScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool TestScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void TestScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    // Test skew and rotation skew on test image

     if(code == EventKeyboard::KeyCode::KEY_S) {
         _deck->shuffleCards();
     }
     else if (code == EventKeyboard::KeyCode::KEY_D)
     {
         _deck->dealCards(_zones, 2);
     }

    //if(code == EventKeyboard::KeyCode::KEY_Q) {
    //    _testImg->setSkewX(_testImg->getSkewX() + 10);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_W) {
    //    _testImg->setSkewX(_testImg->getSkewX() - 10);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_A)
    //{
    //    _testImg->setSkewY(_testImg->getSkewY() + 10);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_S)
    //{
    //    _testImg->setSkewY(_testImg->getSkewY() - 10);
    //}

    //if (code == EventKeyboard::KeyCode::KEY_R)
    //{
    //    _testImg->setRotationSkewX(_testImg->getRotationSkewX() + 10);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_T)
    //{
    //    _testImg->setRotationSkewX(_testImg->getRotationSkewX() - 10);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_F)
    //{
    //    _testImg->setRotationSkewY(_testImg->getRotationSkewY() + 10);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_G)
    //{
    //    _testImg->setRotationSkewY(_testImg->getRotationSkewY() - 10);
    //}

    //if (code == EventKeyboard::KeyCode::KEY_U)
    //{
    //    _testImg->setScaleX(_testImg->getScaleX() + 0.1f);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_I)
    //{
    //    _testImg->setScaleX(_testImg->getScaleX() - 0.1f);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_J)
    //{
    //    _testImg->setScaleY(_testImg->getScaleY() + 0.1f);
    //}
    //else if (code == EventKeyboard::KeyCode::KEY_K)
    //{
    //    _testImg->setScaleY(_testImg->getScaleY() - 0.1f);
    //}
}

void TestScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void TestScene::startSocket(string authToken) {
    
}

TestScene::~TestScene() {}
