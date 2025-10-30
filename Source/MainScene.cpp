#include "MainScene.h"

using namespace ax;
using namespace std;

static int s_sceneID = 1000;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _draggedCard = nullptr;
    _hoveredCard = nullptr;

    loadCardsFromDirectory();

    _mouseListener                = EventListenerMouse::create();
    _mouseListener->onMouseMove   = AX_CALLBACK_1(MainScene::onMouseMove, this);
    _mouseListener->onMouseUp     = AX_CALLBACK_1(MainScene::onMouseUp, this);
    _mouseListener->onMouseDown   = AX_CALLBACK_1(MainScene::onMouseDown, this);
    _mouseListener->onMouseScroll = AX_CALLBACK_1(MainScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

bool MainScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Left click for dragging
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {
        // Check which card was clicked (iterate in reverse for Z-order)
        for (int i = _cards.size() - 1; i >= 0; i--)
        {
            auto card = _cards[i];
            if (card->containsPoint(mousePos))
            {
                _draggedCard = card;

                // Calculate offset between mouse and card center
                auto cardPos = card->getPosition();
                card->setDragOffset(mousePos - cardPos);

                // Store original position for snap-back
                card->setOriginalPosition(cardPos);

                card->setLocalZOrder(100);

                card->startDragging();

                auto it = _cards.begin() + i;
                _cards.erase(it);
                _cards.push_back(card);
                return true;  // Event handled
            }
        }
    }

    // Right click for flipping
    else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        for (int i = _cards.size() - 1; i >= 0; i--)
        {
            auto card = _cards[i];
            if (card->containsPoint(mousePos))
            {
                card->flip();
                return true;
            }
        }
    }

    return false;
}

bool MainScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    _draggedCard = nullptr;  // Stop dragging any card
    return true;
}

bool MainScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // If dragging a card, move it
    if (_draggedCard != nullptr)
    {
        auto dragOffset = _draggedCard->getDragOffset();
        _draggedCard->setPosition(mousePos - dragOffset);

        updateHoverStates(mousePos);
    }
    else
    {
        // Update hover states for visual feedback
        updateHoverStates(mousePos);
    }

    return true;
}

bool MainScene::onMouseScroll(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseScroll detected, X:{}  Y:{}", e->getScrollX(), e->getScrollY());
    return true;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    AXLOGD("Scene: #{} onKeyPressed, keycode: {}", _sceneID, static_cast<int>(code));
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    AXLOGD("onKeyReleased, keycode: {}", static_cast<int>(code));
}

void MainScene::update(float delta)
{
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    }  // switch
}

void MainScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

MainScene::MainScene()
{
    _sceneID = ++s_sceneID;
    AXLOGD("Scene: ctor: #{}", _sceneID);
}

MainScene::~MainScene()
{
    AXLOGD("~Scene: dtor: #{}", _sceneID);

    if (_touchListener)
        _eventDispatcher->removeEventListener(_touchListener);
    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
    _sceneID = -1;
}

Card* MainScene::getCardAtPosition(const ax::Vec2& position)
{
    for (int i = _cards.size() - 1; i >= 0; i--)
    {
        auto card = _cards[i];
        if (card->containsPoint(position))
            return card;
    }
    return nullptr;
}

vector<string> split(const string& str, char delimiter)
{
    vector<string> result;
    stringstream ss(str);
    string item;

    while (getline(ss, item, delimiter))
    {
        result.push_back(item);
    }

    return result;
}

void MainScene::loadCardsFromDirectory() {
    // Get all files in a folder
    auto fileUtils = ax::FileUtils::getInstance();
    vector<string> files;

    string folderPath = "cards/" + cardTypeFolder; 
    files = fileUtils->listFiles(folderPath);
    int numCards = static_cast<int>(files.size());
    int i = 0;
    for (const auto& file : files)
    {
        vector<string> parts = split(file, '/'); //get filename from path
        auto card = Card::create(folderPath + parts.back(), "cards/Card Back 1.png");
        if (card)
        {
            card->setPosition(
                Vec2(origin.x + (i + 1) * visibleSize.width / (numCards + 1), origin.y + visibleSize.height / 2));
            this->addChild(card, i);
            _cards.push_back(card);
        }
        else
        {
            problemLoading("card_front.png or card_back.png");
        }
        i++;
    }
}

void MainScene::updateHoverStates(const ax::Vec2& mousePos)
{
    Card* previousHoveredCard = _hoveredCard;
    _hoveredCard              = getCardAtPosition(mousePos);
    if (previousHoveredCard != _hoveredCard)
    {
        if (previousHoveredCard)
            previousHoveredCard->setHighlight(false);
        if (_hoveredCard)
            _hoveredCard->setHighlight(true);
    }
}
