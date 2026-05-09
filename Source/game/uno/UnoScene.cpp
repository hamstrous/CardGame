#include "UnoScene.h"
#include "core/rule/command/DealCommand.h"
#include "core/rule/command/ShuffleCommand.h"
#include "core/rule/command/MainGameCommand.h"

#include "core/view/View.h"
#include "core/view/Player.h"
#include "core/model/StateManager.h"

#include "core/network/HttpRequestHandler.h"

#include "utils/helper.h"

using namespace ax;
using namespace ax::network;
using namespace std;
using namespace helper;

bool UnoScene::init()
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
    _mouseListener->onMouseMove = AX_CALLBACK_1(UnoScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(UnoScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(UnoScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(UnoScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(UnoScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    _websocketListener = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(UnoScene::onWebSocketMessage, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 1);

    _cardEventListener = EventListenerCard::create();
    _cardEventListener->onCardClicked = AX_CALLBACK_1(UnoScene::onPlayCard, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_cardEventListener, 2);

    _socketManager = SocketNetworkManager::getInstance();

    StateManager::getInstance()->setGameState(new GameState());
    _gameState = StateManager::getInstance()->getGameState();


    setUpObjects();

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void UnoScene::update(float delta) {}

void UnoScene::setUpObjects() {
    _playerCount = 4;
    _playerId = 0;
    auto handPositionList = vector<Vec2>{
        Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 300),  // bottom
        Vec2(visibleSize.width / 2 + origin.x - 400, visibleSize.height / 2 + origin.y),  // right
        Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 300),  // top
        Vec2(visibleSize.width / 2 + origin.x + 400, visibleSize.height / 2 + origin.y)   // left
    };
    int listIndex = 0;
    for (int i = _playerId; i < _playerCount; i++)
    {
        Zone* playerHand = Zone::create(new ZoneData());
        // play the zone in a circular layout around the center of the screen, the bottom player is the client player,
        // the others are opponents, going clockwise
        if (listIndex % 2 == 1)
            playerHand->setRotation(90);
        playerHand->setPosition(handPositionList[listIndex++]);
        playerHand->setContentSize(Size(300, 100));
        this->addChild(playerHand);
        _playerHands.pushBack(playerHand);
        playerHand->lockInput(); 
    }
    for (int i = 0; i < _playerId; i++)
    {
        Zone* playerHand = Zone::create(new ZoneData());
        if (listIndex % 2 == 0)
            playerHand->setRotation(90);
        playerHand->setPosition(handPositionList[listIndex++]);
        playerHand->setContentSize(Size(300, 100));
        this->addChild(playerHand);
        _playerHands.pushBack(playerHand);
        playerHand->lockInput(); 
    }

    // load all card files from content/card/uno


    _deck = Deck::create(new ZoneData());
    _deck->setPosition(150, visibleSize.height - 100);
    _deck->setContentSize(Size(150, 100));
    this->addChild(_deck);

    int cardIndex = 0;
    auto cardFiles = getFileNamesInFolder("card/uno");
    for (const auto& cardFile : cardFiles)
    {
        AXLOGD("Loading card file: {}", cardFile);
        Card* card = Card::create(new CardData("card/uno/" + cardFile, "card/Card Back 1.png"));
        _cards.pushBack(card);
        card->setId(cardIndex++);
        this->addChild(card); // add to scene first to ensure the card can be move, and no problem with the remove parent
        _deck->moveCardToThisZone(card);
    }

    _discardPile = Zone::create(new ZoneData());
    _discardPile->setPosition(visibleSize / 2);
    _discardPile->setContentSize(Size(150, 150));
    this->addChild(_discardPile);
    
}


bool UnoScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool UnoScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool UnoScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void UnoScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {}

void UnoScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void UnoScene::onWebSocketMessage(EventWebSocket* event) {}

void UnoScene::onAllPlayersReady(EventWebSocket* event) {}

void UnoScene::onCardPlayed(EventWebSocket* event) {}

void UnoScene::onPlayCard(EventCard* event) {}

void UnoScene::onEnter() {
    Scene::onEnter();
}

UnoScene::~UnoScene() {}
