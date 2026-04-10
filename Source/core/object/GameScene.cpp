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

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

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

    Card* card = Card::create(new CardData("card/uno/0_blue.png", "card/Card Back 1.png"));

    this->addChild(card);
    card->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100));
    card->setContentSize(Size(100, 150));

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void GameScene::update(float delta) {}

GameScene::~GameScene() {}
