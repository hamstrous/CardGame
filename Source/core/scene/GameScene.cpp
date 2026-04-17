#include "GameScene.h"

using namespace ax;
using namespace std;

bool GameScene::init()
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

    // Some templates (uncomment what you  need)

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(GameScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(GameScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(GameScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    setUpScene();

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void GameScene::update(float delta) {}

void GameScene::setUpScene() {
    //Card* card = Card::create(new CardData("card/uno/0_blue.png", "card/Card Back 1.png"));

    //this->addChild(card);
    //card->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100));
    //card->setContentSize(Size(100, 150));
    //card->lockInput();

    //Card* card2 = Card::create(new CardData("card/uno/0_red.png", "card/Card Back 1.png"));

    //this->addChild(card2);
    //card2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 200));
    //card2->setContentSize(Size(100, 150));

    //Card* card3 = Card::create(new CardData("card/uno/0_green.png", "card/Card Back 1.png"));
    //this->addChild(card3);
    //card3->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 300));
    //card3->setContentSize(Size(100, 150));

    //

    // Set up 2 zone on opposite sides
    Zone* zone = Zone::create(new ZoneData());
    this->addChild(zone);
    zone->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 200));
    zone->setContentSize(Size(300, 150));

    Zone* zone2 = Zone::create(new ZoneData());
    this->addChild(zone2);
    zone2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 200));
    zone2->setContentSize(Size(300, 150));
    zone2->setRotation(180);

    // Set up 8 cards 4 for each side
    for (auto i : {"0", "1"})
        for (auto color : {"blue", "red", "green", "yellow"})
        {
            Card* card = Card::create(new CardData("card/uno/" + string(i) + "_" + color + ".png", "card/Card Back 1.png"));
            this->addChild(card);
            card->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            card->setContentSize(Size(100, 150));
            if (i == "0") {
                card->moveToZone(zone);
            }
            else
            {
                card->moveToZone(zone2);
            }
        }

}


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

GameScene::~GameScene() {}
