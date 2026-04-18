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

    setUpObjects();

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void GameScene::update(float delta) {}

void GameScene::setUpObjects() {
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
    zone->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 300));
    zone->setContentSize(Size(300, 150));

    Zone* zone2 = Zone::create(new ZoneData());
    this->addChild(zone2);
    zone2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 300));
    zone2->setContentSize(Size(300, 150));
    zone2->setRotation(180);

    Zone* zone3 = Zone::create(new ZoneData());
    this->addChild(zone3);
    zone3->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    zone3->setContentSize(Size(300, 300));

    zones.pushBack(zone);
    zones.pushBack(zone2);
    zones.pushBack(zone3);

    // Set up 8 cards 4 for each side
    for (auto i : {"0", "1"})
        for (auto color : {"blue", "red", "green", "yellow"})
        {
            Card* card = Card::create(new CardData("card/uno/" + string(i) + "_" + color + ".png", "card/Card Back 1.png"));
            this->addChild(card);
            card->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
            card->setContentSize(Size(100, 150));
            //if (i == "0") {
            //    card->moveToZone(zone);
            //}
            //else
            //{
            //    card->moveToZone(zone2);
            //}
            cards.pushBack(card);
            card->setName(string(color) + string(i));
        }

}

void GameScene::setUpRule() {
    for (int i = 0; i < cards.size(); ++i)
    {
        cards[i]->lockInput();
    }

    for (int i = 0; i < zones.size(); ++i)
    {
        zones[i]->lockInput();
    }

    Command* dealCommand = new DealCommand(cards, zones);
    Command* shuffleCommand = new ShuffleCommand(cards);
    std::vector<Command*> commands{shuffleCommand, dealCommand};
    Phase* dealPhase = new Phase(commands);

    Turn* initTurn = new Turn(dealPhase);
    Rule* rule = new Rule(initTurn, nullptr, nullptr);
    rule->startRule();

    this->addChild(rule);
    rule->addChild(initTurn);
    initTurn->addChild(dealPhase);
    dealPhase->addChild(dealCommand);
    dealPhase->addChild(shuffleCommand);
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

void GameScene::onEnter() {
    Scene::onEnter();
    setUpRule();
}

GameScene::~GameScene() {}
