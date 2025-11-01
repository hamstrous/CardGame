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

    _draggedObjects.clear();
    _hoveredObject = nullptr;

    // Add background that fits the screen
    auto background = Sprite::create("background.png");  // Change to your background filename
    if (background)
    {
        // Position at center of screen
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // Scale to fit screen (maintains aspect ratio, covers screen)
        float scaleX = visibleSize.width / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        float scale  = std::max(scaleX, scaleY);  // Use max to cover entire screen
        background->setScale(scale);

        // Add as background (lowest Z-order)
        this->addChild(background, -100);
    }
    else
    {
        problemLoading("background.png");
    }

    _selectionRectangle = DrawNode::create();
    this->addChild(_selectionRectangle, 1000);
    _selectionStartPoint = Vec2::ZERO;

    loadCardsFromDirectory();
    loadTables();
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
        sortObjectsByZOrder(_cards);
        for (int i = _cards.size() - 1; i >= 0; i--)
        {
            auto card = _cards[i];
            if (card->containsPoint(mousePos) && card->isDraggable())
            {
                if (!contains(_selectedObjects, card))
                {
                    selectObjectsClear();
                    if (initDrag(card, mousePos))
                    {
                        card->setLocalZOrder(_cardClickCount++);
                    }
                }
                else
                {
                    sortObjectsByZOrder(_selectedObjects);
                    for (auto selectedCard : _selectedObjects)
                    {
                        if (initDrag(selectedCard, mousePos))
                        {
                            selectedCard->setLocalZOrder(_cardClickCount++);
                        }
                    }
                }

                for (auto table : _tables)
                {
                    for (auto draggedCard : _draggedObjects)
                        if (table->containsPoint(draggedCard->getPosition()))
                        {
                            table->removeCard(dynamic_cast<Card*>(draggedCard));
                        }
                }
                return true;
                        }
        }
        selectObjectsClear();
        for (int i = _tables.size() - 1; i >= 0; i--)
        {
            auto table = _tables[i];
            if (table->containsPoint(mousePos))
            {
                if (initDrag(table, mousePos))
                {
                    return true;
                }
            }
        }
        _selectionStartPoint = mousePos;
    }
    // Right click for flipping
    else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
    {
        for (int i = _cards.size() - 1; i >= 0; i--)
        {
            auto card = _cards[i];
            if (card->containsPoint(mousePos))
            { 
                if (!contains(_selectedObjects, card))
                {
                    card->flip();
                    selectObjectsClear();
                }
                else
                {
                    for (auto selectedCard : _selectedObjects)
                    {
                        dynamic_cast<Card*>(selectedCard)->flip();
                    }
                }
                return true;
            }
        }
    }

    return false;
}

bool MainScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());
    vector<Card*> _draggedCards;
    sortObjectsByPosition(_draggedObjects);
    for (auto obj : _draggedObjects)
    {
        if (auto card = dynamic_cast<Card*>(obj))
        {
            _draggedCards.push_back(card);
        }
    }
    for (auto table : _tables)
    {
        if (!_draggedObjects.empty() && table->containsPoint(mousePos))
        {
            table->addCard(_draggedCards, mousePos);
            break;
        }
    }
    _draggedObjects.clear();
    _selectionRectangle->clear();
    _selectionStartPoint = Vec2::ZERO;
    return true;
}

bool MainScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    if (!_draggedObjects.empty())
    {
        for (auto draggedObject : _draggedObjects)
        {
            auto dragOffset = draggedObject->getDragOffset();
            draggedObject->setPosition(mousePos - dragOffset);
        }
    }
    else
    {
        if (_selectionStartPoint != Vec2::ZERO)
        {
            updateSelectionRectangle(mousePos);
        }
        else
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

DraggableObject* MainScene::getObjectAtPosition(const ax::Vec2& position)
{
    for (int i = _cards.size() - 1; i >= 0; i--)
    {
        auto card = _cards[i];
        if (card->containsPoint(position))
            return card;
    }
    for (int i = _tables.size() - 1; i >= 0; i--)
    {
        auto table = _tables[i];
        if (table->containsPoint(position))
            return table;
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

void MainScene::loadCardsFromDirectory()
{
    // Get all files in a folder
    auto fileUtils = ax::FileUtils::getInstance();
    vector<string> files;

    string folderPath = "cards/" + cardTypeFolder;
    files             = fileUtils->listFiles(folderPath);
    int numCards      = static_cast<int>(files.size());
    int i             = 0;
    for (const auto& file : files)
    {
        vector<string> parts = split(file, '/');  // get filename from path
        auto card            = Card::create(folderPath + parts.back(), "cards/Card Back 1.png");
        if (card)
        {
            card->setPosition(
                Vec2(origin.x + (i + 1) * visibleSize.width / (numCards + 1), origin.y + visibleSize.height / 2));
            this->addChild(card, _cardClickCount++);
            _cards.push_back(card);
            Vec2 cardWorld   = card->convertToWorldSpace(Vec2::ZERO);
            Vec2 parentWorld = card->getParent()->convertToWorldSpace(card->getPosition());
        }
        else
        {
            problemLoading("card_front.png or card_back.png");
        }
        i++;
    }
}

void MainScene::loadTables()
{
    _tables.push_back(Table::create("tables/table_blue.png"));
    _tables.push_back(Table::create("tables/table_red.png"));
    _tables[0]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + _tables[0]->getContentSize().y / 2));
    _tables[1]->setPosition(
        Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - _tables[1]->getContentSize().y / 2));
    for (size_t i = 0; i < _tables.size(); i++)
    {
        auto table = _tables[i];
        if (!table)
        {
            problemLoading("table.png");
            continue;
        }
        this->addChild(table, -1);
    }
}

void MainScene::updateHoverStates(const ax::Vec2& mousePos)
{
    sortObjectsByZOrder(_cards);

    DraggableObject* previousHovered = _hoveredObject;
    _hoveredObject                   = getObjectAtPosition(mousePos);

    if (previousHovered != _hoveredObject)
    {
        if (previousHovered && !contains(_selectedObjects, previousHovered))
            previousHovered->setHighlight(false);
        if (_hoveredObject)
            _hoveredObject->setHighlight(true);
    }
}

bool MainScene::initDrag(DraggableObject* obj, const ax::Vec2& mousePos)
{
    if (!obj || !obj->isDraggable())
        return false;

    auto objPos = obj->getPosition();
    obj->setDragOffset(mousePos - objPos);
    obj->setOriginalPosition(objPos);
    obj->startDragging();

    _draggedObjects.push_back(obj);
    return true;
}

void MainScene::updateSelectionRectangle(const ax::Vec2& currentPoint)
{
    _selectionRectangle->clear();
    Vec2 corners[4] = {
        _selectionStartPoint,
        Vec2(currentPoint.x, _selectionStartPoint.y),
        currentPoint,
        Vec2(_selectionStartPoint.x, currentPoint.y),
    };
    _selectionRectangle->drawSolidPoly(corners, 4, Color4F(0, 0, 1, 0.3f));

    for (auto card : _cards)
    {
        // Check if card's bounding box inside selection rectangle
        Rect cardBox = card->getBoundingBox();
        Rect selectionBox =
            Rect(min(_selectionStartPoint.x, currentPoint.x), min(_selectionStartPoint.y, currentPoint.y),
                 abs(currentPoint.x - _selectionStartPoint.x), abs(currentPoint.y - _selectionStartPoint.y));

        if (selectionBox.containsPoint(Vec2(cardBox.getMinX(), cardBox.getMinY())) &&
            selectionBox.containsPoint(Vec2(cardBox.getMaxX(), cardBox.getMinY())) &&
            selectionBox.containsPoint(Vec2(cardBox.getMaxX(), cardBox.getMaxY())) &&
            selectionBox.containsPoint(Vec2(cardBox.getMinX(), cardBox.getMaxY())))
        {
            card->setHighlight(true);
            if (!contains(_selectedObjects, card))
            {
                _selectedObjects.push_back(card);
            }
        }
        else
        {
            // Deselect the card
            card->setHighlight(false);
            if (contains(_selectedObjects, card))
            {
                auto it = std::find(_selectedObjects.begin(), _selectedObjects.end(), card);
                _selectedObjects.erase(it);
            }
        }
    }
}
