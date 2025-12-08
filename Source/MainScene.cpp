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

template <typename U, typename T>
std::vector<U*> castVector(const std::vector<T*>& src)
{
    std::vector<U*> result;
    result.reserve(src.size());

    for (T* item : src)
        result.push_back(item);

    return result;
}

ax::MenuItem* createMenuEntry(const std::string& text, const ax::ccMenuCallback& callback)
{
    const ax::Vec2 CONTEXT_MENU_BUTTON_SIZE = ax::Vec2(130, 30);

    auto bgNormal   = ax::Sprite::create("ui/button_background.png");
    auto bgSelected = ax::Sprite::create("ui/button_background_selected.png");

    // Scale the sprites to fit the target size
    bgNormal->setScale(CONTEXT_MENU_BUTTON_SIZE.x / bgNormal->getContentSize().width,
                       CONTEXT_MENU_BUTTON_SIZE.y / bgNormal->getContentSize().height);

    bgSelected->setScale(CONTEXT_MENU_BUTTON_SIZE.x / bgSelected->getContentSize().width,
                         CONTEXT_MENU_BUTTON_SIZE.y / bgSelected->getContentSize().height);

    // Create the menu item with the scaled sprites
    auto item = ax::MenuItemSprite::create(bgNormal, bgSelected, callback);
    item->setContentSize(CONTEXT_MENU_BUTTON_SIZE);

    // Label
    auto label = ax::Label::createWithSystemFont(text, "Arial", 20);
    label->setTextColor(ax::Color4B::BLACK);
    // Align left, center vertically:
    // reference point: left side + some padding
    float padding = 10.0f;
    label->setAnchorPoint({0, 0.5f});
    label->setPosition({padding, CONTEXT_MENU_BUTTON_SIZE.y * 0.5f});

    item->addChild(label);

    return item;
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
        this->addChild(background, -10000);
    }
    else
    {
        problemLoading("background.png");
    }

    _selectionRectangle = DrawNode::create();
    this->addChild(_selectionRectangle, 1000);
    _selectionStartPoint = Vec2::ZERO;

    auto _addMenuBackground = Sprite::create("ui/button_background.png");
    auto _addCard           = createMenuEntry("Add Card", [=](auto) { createSpecificAddMenu("cards"); });
    auto _addDeck           = createMenuEntry("Add Deck", [=](auto) { createSpecificAddMenu("decks"); });
    auto _addTable          = createMenuEntry("Add Table", [=](auto) { createSpecificAddMenu("tables"); });
    auto _addRack           = createMenuEntry("Add Rack", [=](auto) { createSpecificAddMenu("racks"); });
    auto _addCounter        = createMenuEntry("Add Counter", [=](auto) { createSpecificAddMenu("counters"); });

    _addMenuItems.pushBack(_addCard);
    _addMenuItems.pushBack(_addDeck);
    _addMenuItems.pushBack(_addTable);
    _addMenuItems.pushBack(_addRack);
    _addMenuItems.pushBack(_addCounter);

    _subAddFolder["cards"]    = {"uno/", "standard/", "ascension/"};
    _subAddFolder["decks"]    = {};
    _subAddFolder["tables"]   = {};
    _subAddFolder["racks"]    = {};
    _subAddFolder["counters"] = {};

    _addMenu = ax::Menu::createWithArray(_addMenuItems);
    _addMenu->alignItemsVerticallyWithPadding(0);
    _addMenu->setVisible(false);
    //_addMenu->setAnchorPoint(Vec2(0, 0));
    this->addChild(_addMenu, 10000);

    loadCardsFromDirectory();
    loadRacks();
    loadDecks();
    loadTables();
    loadCountersFromDirectory();
    getAllObjects(_objects);

    _mouseListener                = EventListenerMouse::create();
    _mouseListener->onMouseMove   = AX_CALLBACK_1(MainScene::onMouseMove, this);
    _mouseListener->onMouseUp     = AX_CALLBACK_1(MainScene::onMouseUp, this);
    _mouseListener->onMouseDown   = AX_CALLBACK_1(MainScene::onMouseDown, this);
    _mouseListener->onMouseScroll = AX_CALLBACK_1(MainScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(MainScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(MainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);

    // Add custom event listener for window close event
    auto closeListener = EventListenerCustom::create(EVENT_COME_TO_BACKGROUND, [this](EventCustom* event) {
        AXLOGD("Window close event received, cleaning up...");
        this->cleanup();
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(closeListener, this);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();
    // #if _DEBUG
    //     _CrtCheckMemory();
    // #endif

    return true;
}

bool MainScene::onMouseDown(Event* event)
{
    if (isMoveMode)
        return onMoveModeMouseDown(event);
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // In connect mode, check for connection line clicks to disconnect
    if (isConnectMode)
    {
        ConnectionInfo* clickedConnection = getConnectionAtPosition(mousePos);
        if (clickedConnection)
        {
            // Disconnect the holder from the deck
            clickedConnection->deck->disconnectHolder(clickedConnection->holder);

            // Redraw all connections
            _connectionLines->clear();
            _connectionInfos.clear();

            for (auto deck : _decks)
            {
                const std::vector<Holder*>& connectedHolders = deck->getConnectedHolders();
                ax::Vec2 deckPos                             = deck->getPosition();

                for (auto holder : connectedHolders)
                {
                    ax::Vec2 holderPos = holder->getPosition();
                    _connectionLines->drawLine(deckPos, holderPos, ax::Color4F::GREEN, 3.0f);
                    _connectionLines->drawSolidCircle(deckPos, 5.0f, 0.0f, 16, ax::Color4F::GREEN);
                    _connectionLines->drawSolidCircle(holderPos, 5.0f, 0.0f, 16, ax::Color4F::GREEN);

                    ConnectionInfo connInfo;
                    connInfo.deck     = deck;
                    connInfo.holder   = holder;
                    connInfo.startPos = deckPos;
                    connInfo.endPos   = holderPos;
                    _connectionInfos.push_back(connInfo);
                }
            }

            return true;  // Consume the event, stay in connect mode
        }

        // Check if clicking on a deck to start connection drag
        for (auto deck : _decks)
        {
            if (deck->containsPoint(mousePos))
            {
                _isConnectDragging = true;
                _connectSourceDeck = deck;

                // Create drag line if it doesn't exist
                if (!_connectDragLine)
                {
                    _connectDragLine = ax::DrawNode::create();
                    this->addChild(_connectDragLine, 10001);  // Above connection lines
                }

                return true;
            }
        }

        return true;  // Consume the event, stay in connect mode
    }

    // Exit zoom mode on any mouse click
    if (isZoomMode && _zoomedCard)
    {
        _zoomedCard->unzoom();
        isZoomMode  = false;
        _zoomedCard = nullptr;
        _connectionInfos.clear();
    }

    _addMenu->setVisible(false);
    if (_addSpecificMenu)
        _addSpecificMenu->setVisible(false);

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
                if (isCtrlPressed)
                {
                    // Ctrl+Click: toggle selection
                    if (contains(_selectedObjects, card))
                    {
                        // Remove from selection
                        auto it = std::find(_selectedObjects.begin(), _selectedObjects.end(), card);
                        _selectedObjects.erase(it);
                        card->setHighlight(false);
                        return true;
                    }
                    else
                    {
                        // Add to selection
                        _selectedObjects.push_back(card);
                        card->setHighlight(true);
                    }
                }
                else
                {
                    // Normal click: replace selection
                    if (!contains(_selectedObjects, card))
                    {
                        clearSelectObjects();
                        _selectedObjects.push_back(card);
                    }
                }
                sortObjectsByZOrder(_selectedObjects);
                for (auto selectedCard : _selectedObjects)
                {
                    if (initDrag(selectedCard, mousePos))
                    {
                        pushToTop(selectedCard);
                    }
                }

                vector<Card*> _draggedCards;
                for (auto obj : _draggedObjects)
                {
                    if (auto card = dynamic_cast<Card*>(obj))
                    {
                        _draggedCards.push_back(card);
                    }
                }
                for (auto rack : _racks)
                {
                    rack->removeCard(_draggedCards);
                }
                for (auto deck : _decks)
                {
                    deck->removeCard(_draggedCards);
                }
                for (auto table : _tables)
                {
                    table->removeCard(_draggedCards);
                }
                return true;
            }
        }
        clearSelectObjects();
        for (int i = _racks.size() - 1; i >= 0; i--)
        {
            auto rack = _racks[i];
            if (rack->containsPoint(mousePos))
            {
                if (initDrag(rack, mousePos))
                {
                    return true;
                }
            }
        }
        for (auto deck : _decks)
        {
            if (deck->containsPoint(mousePos))
            {
                deck->shuffle();
                return true;
            }
        }
        _selectionStartPoint = mousePos;
    }
    // Right click
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
                    clearSelectObjects();
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
        for (auto deck : _decks)
        {
            if (deck->containsPoint(mousePos))
            {
                deck->dealSmoothly();
                return true;
            }
        }

        // Right click to create add menu pop up
        _addMenu->setPosition(mousePos);
        _addMenu->setVisible(true);
    }

    return false;
}

bool MainScene::onMouseUp(Event* event)
{
    if (isMoveMode)
        return onMoveModeMouseUp(event);
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Handle connect mode drag completion
    if (_isConnectDragging && _connectSourceDeck)
    {
        _isConnectDragging = false;

        // Clear the drag line
        if (_connectDragLine)
        {
            _connectDragLine->clear();
        }

        // Check if mouse is over a holder
        Holder* targetHolder = nullptr;

        // Check racks
        for (auto rack : _racks)
        {
            if (rack->containsPoint(mousePos))
            {
                targetHolder = rack;
                break;
            }
        }

        // Check decks (if not already found)
        if (!targetHolder)
        {
            for (auto deck : _decks)
            {
                if (deck->containsPoint(mousePos) && deck != _connectSourceDeck)
                {
                    targetHolder = deck;
                    break;
                }
            }
        }

        // Check tables (if not already found)
        if (!targetHolder)
        {
            for (auto table : _tables)
            {
                if (table->containsPoint(mousePos))
                {
                    targetHolder = table;
                    break;
                }
            }
        }

        // If we found a holder, connect it to the deck
        if (targetHolder)
        {
            _connectSourceDeck->connectHolder(targetHolder);

            // Redraw all connections
            _connectionLines->clear();
            _connectionInfos.clear();

            for (auto deck : _decks)
            {
                const std::vector<Holder*>& connectedHolders = deck->getConnectedHolders();
                ax::Vec2 deckPos                             = deck->getPosition();

                for (auto holder : connectedHolders)
                {
                    ax::Vec2 holderPos = holder->getPosition();
                    _connectionLines->drawLine(deckPos, holderPos, ax::Color4F::GREEN, 3.0f);
                    _connectionLines->drawSolidCircle(deckPos, 5.0f, 0.0f, 16, ax::Color4F::GREEN);
                    _connectionLines->drawSolidCircle(holderPos, 5.0f, 0.0f, 16, ax::Color4F::GREEN);

                    ConnectionInfo connInfo;
                    connInfo.deck     = deck;
                    connInfo.holder   = holder;
                    connInfo.startPos = deckPos;
                    connInfo.endPos   = holderPos;
                    _connectionInfos.push_back(connInfo);
                }
            }
        }

        _connectSourceDeck = nullptr;
        return true;
    }

    vector<Card*> _draggedCards;
    for (auto obj : _draggedObjects)
    {
        if (auto card = dynamic_cast<Card*>(obj))
        {
            _draggedCards.push_back(card);
        }
    }
    _draggedObjects.clear();
    _selectionRectangle->clear();
    _selectionStartPoint = Vec2::ZERO;

    // Add cards to holders
    for (auto rack : _racks)
    {
        if (!_draggedCards.empty() && rack->containsPoint(mousePos))
        {
            rack->addCard(_draggedCards, mousePos);
            return true;
        }
    }

    for (auto deck : _decks)
    {
        if (!_draggedCards.empty() && deck->containsPoint(mousePos))
        {
            deck->addCard(_draggedCards);
            return true;
        }
    }

    for (auto table : _tables)
    {
        if (!_draggedCards.empty() && table->containsPoint(mousePos))
        {
            table->addCard(_draggedCards, mousePos);
            return true;
        }
    }

    return true;
}

bool MainScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Handle connect mode dragging
    if (_isConnectDragging && _connectSourceDeck && _connectDragLine)
    {
        _connectDragLine->clear();
        ax::Vec2 deckPos = _connectSourceDeck->getPosition();
        _connectDragLine->drawLine(deckPos, mousePos, ax::Color4F::YELLOW, 3.0f);
        _connectDragLine->drawSolidCircle(deckPos, 5.0f, 0.0f, 16, ax::Color4F::YELLOW);
        return true;
    }

    if (_addMenu && _addMenu->isVisible())
    {
        // iterate menu items and set selected/unselected based on mouse position
        bool anySelected = false;
        for (auto item : _addMenuItems)
        {
            // getBoundingBox() is in parent's coordinates (scene), mousePos is in world coords
            // ensure mousePos is in same coordinate space (it is — screen coords used here)
            if (item->getBoundingBox().containsPoint(mousePos))
            {
                item->selected();  // show selected sprite
                anySelected = true;
            }
            else
            {
                item->unselected();  // show normal sprite
            }
            break;
        }
        if (anySelected)
            return true;
    }

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

bool MainScene::onMoveModeMouseDown(ax::Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());

    // Exit zoom mode on any mouse click
    if (isZoomMode && _zoomedCard)
    {
        _zoomedCard->unzoom();
        isZoomMode  = false;
        _zoomedCard = nullptr;
    }

    // Exit connect mode on any mouse click
    if (isConnectMode)
    {
        isConnectMode = false;
        if (_connectionLines)
        {
            _connectionLines->clear();
        }
        _connectionInfos.clear();

        // Clean up connect drag state
        _isConnectDragging = false;
        _connectSourceDeck = nullptr;
        if (_connectDragLine)
        {
            _connectDragLine->clear();
        }
    }

    getAllObjects(_objects);
    // Left click for dragging
    if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
    {

        // Check which object was clicked (iterate in reverse for Z-order)
        for (int i = _objects.size() - 1; i >= 0; i--)
        {
            auto obj = _objects[i];
            if (obj->containsPoint(mousePos) && obj->isDraggable())
            {
                if (isCtrlPressed)
                {
                    // Ctrl+Click: toggle selection
                    if (contains(_selectedObjects, obj))
                    {
                        // Remove from selection
                        auto it = std::find(_selectedObjects.begin(), _selectedObjects.end(), obj);
                        _selectedObjects.erase(it);
                        obj->setHighlight(false);
                        return true;
                    }
                    else
                    {
                        // Add to selection
                        _selectedObjects.push_back(obj);
                        obj->setHighlight(true);
                    }
                }
                else
                {
                    // Normal click: replace selection
                    if (!contains(_selectedObjects, obj))
                    {
                        clearSelectObjects();
                        _selectedObjects.push_back(obj);
                    }
                }
                sortObjectsByZOrder(_selectedObjects);
                for (auto selectedObject : _selectedObjects)
                {
                    if (initDrag(selectedObject, mousePos))
                    {
                        pushToTop(selectedObject);
                    }
                }
                vector<Card*> _draggedCards;
                for (auto obj : _draggedObjects)
                {
                    if (auto card = dynamic_cast<Card*>(obj))
                    {
                        _draggedCards.push_back(card);
                    }
                }
                for (auto rack : _racks)
                {
                    rack->removeCard(_draggedCards);
                }
                for (auto deck : _decks)
                {
                    deck->removeCard(_draggedCards);
                }
                for (auto table : _tables)
                {
                    table->removeCard(_draggedCards);
                }
                return true;
            }
        }
        clearSelectObjects();
        _selectionStartPoint = mousePos;
    }
    return false;
}

bool MainScene::onMoveModeMouseUp(ax::Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    auto mousePos = Vec2(e->getCursorX(), e->getCursorY());
    vector<Card*> _draggedCards;
    for (auto obj : _draggedObjects)
    {
        if (auto card = dynamic_cast<Card*>(obj))
        {
            _draggedCards.push_back(card);
        }
    }
    _draggedObjects.clear();
    _selectionRectangle->clear();
    _selectionStartPoint = Vec2::ZERO;

    return true;
}

bool MainScene::onMoveModeMouseMove(ax::Event* event)
{
    return false;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    // Track Ctrl key state
    if (code == EventKeyboard::KeyCode::KEY_CTRL || code == EventKeyboard::KeyCode::KEY_LEFT_CTRL ||
        code == EventKeyboard::KeyCode::KEY_RIGHT_CTRL)
    {
        isCtrlPressed = true;
    }

    switch (code)
    {
    case EventKeyboard::KeyCode::KEY_R:
        // Reveal cards up
        for (auto obj : _selectedObjects)
        {
            Card* card = dynamic_cast<Card*>(obj);
            if (card)
                card->setFaceUp(true);
        }
        break;
    case EventKeyboard::KeyCode::KEY_H:
        // Hide cards down
        for (auto obj : _selectedObjects)
        {
            Card* card = dynamic_cast<Card*>(obj);
            if (card)
                card->setFaceUp(false);
        }
        break;
    case EventKeyboard::KeyCode::KEY_Q:
        // Rotate left
        for (auto obj : _selectedObjects)
        {
            if (isMoveMode)
            {
                obj->rotate(-90.0f);
            }
            else
            {
                Card* card = dynamic_cast<Card*>(obj);
                if (card)
                    card->rotate(-90.0f);
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_E:
        // Roatate right
        for (auto obj : _selectedObjects)
        {
            if (isMoveMode)
            {
                obj->rotate(90.0f);
            }
            else
            {
                Card* card = dynamic_cast<Card*>(obj);
                if (card)
                    card->rotate(90.0f);
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_U:
        // Rotate left
        for (auto obj : _selectedObjects)
        {
            if (isMoveMode)
            {
                obj->rotateSmooth(-10.0f);
            }
            else
            {
                Card* card = dynamic_cast<Card*>(obj);
                if (card)
                    card->rotateSmooth(-10.0f);
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_O:
        // Roatate right
        for (auto obj : _selectedObjects)
        {
            if (isMoveMode)
            {
                obj->rotateSmooth(10.0f);
            }
            else
            {
                Card* card = dynamic_cast<Card*>(obj);
                if (card)
                    card->rotateSmooth(10.0f);
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_D:
        for (auto obj : _selectedObjects)
        {
            auto objIt = std::find(_objects.begin(), _objects.end(), obj);
            _objects.erase(objIt);

            // Disconnect holder from all decks before deletion
            Holder* holder = dynamic_cast<Holder*>(obj);
            Deck* deck = dynamic_cast<Deck*>(obj);
            if (holder)
            {
                holder->clearCards();
                for (auto deck : _decks)
                {
                    deck->disconnectHolder(holder);
                }
            }

            if (deck)
            {
                for (auto table : _tables)
                {
                    if (table->getDiscardDeck() == deck)
                    {
                        table->clearDiscardDeck();
                    }
                }
            }

            if (dynamic_cast<Table*>(obj))
            {
                auto it = std::find(_tables.begin(), _tables.end(), dynamic_cast<Table*>(obj));
                _tables.erase(it);
            }
            else if (dynamic_cast<Deck*>(obj))
            {
                auto it = std::find(_decks.begin(), _decks.end(), dynamic_cast<Deck*>(obj));
                _decks.erase(it);
            }
            else if (dynamic_cast<Rack*>(obj))
            {
                auto it = std::find(_racks.begin(), _racks.end(), dynamic_cast<Rack*>(obj));
                _racks.erase(it);
            }
            else if (dynamic_cast<Counter*>(obj))
            {
                auto it = std::find(_counters.begin(), _counters.end(), dynamic_cast<Counter*>(obj));
                _counters.erase(it);
            }
            else if (dynamic_cast<Card*>(obj))
            {
                auto it = std::find(_cards.begin(), _cards.end(), dynamic_cast<Card*>(obj));
                _cards.erase(it);
            }

            if (isMoveMode)
            {
                removeChild(obj);
            }
            else
            {
                Card* card = dynamic_cast<Card*>(obj);
                if (card)
                    removeChild(card);
            }
        }
        _selectedObjects.clear();
        _hoveredObject = nullptr;
        break;
    case EventKeyboard::KeyCode::KEY_Z:
        // Exit connect mode if active
        if (isConnectMode)
        {
            isConnectMode = false;
            if (_connectionLines)
            {
                _connectionLines->clear();
            }
            _connectionInfos.clear();

            // Clean up connect drag state
            _isConnectDragging = false;
            _connectSourceDeck = nullptr;
            if (_connectDragLine)
            {
                _connectDragLine->clear();
            }
        }

        // Zoom feature: only if exactly one card is selected and not in zoom mode
        if (!isZoomMode && _selectedObjects.size() == 1)
        {
            Card* card = dynamic_cast<Card*>(_selectedObjects[0]);
            if (card)
            {
                isZoomMode            = true;
                _zoomedCard           = card;
                ax::Vec2 screenCenter = ax::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
                card->zoomToCenter(screenCenter, 5.0f);
            }
        }
        else if (isZoomMode && _zoomedCard)
        {
            // Exit zoom mode
            _zoomedCard->unzoom();
            isZoomMode  = false;
            _zoomedCard = nullptr;
        }
        break;
    case EventKeyboard::KeyCode::KEY_B:
        // Exit zoom mode if active
        if (isZoomMode && _zoomedCard)
        {
            _zoomedCard->unzoom();
            isZoomMode  = false;
            _zoomedCard = nullptr;
        }

        if (!isConnectMode)
        {
            isConnectMode = true;
            if (!_connectionLines)
            {
                _connectionLines = ax::DrawNode::create();
                this->addChild(_connectionLines, 10000);  // High z-order to draw on top
            }

            // Clear previous lines and connection info
            _connectionLines->clear();
            _connectionInfos.clear();

            // Draw lines from each deck to its connected holders
            for (auto deck : _decks)
            {
                const std::vector<Holder*>& connectedHolders = deck->getConnectedHolders();
                ax::Vec2 deckPos                             = deck->getPosition();

                for (auto holder : connectedHolders)
                {
                    ax::Vec2 holderPos = holder->getPosition();
                    // Draw line with proper thickness parameter
                    _connectionLines->drawLine(deckPos, holderPos, ax::Color4F::GREEN, 3.0f);
                    _connectionLines->drawSolidCircle(deckPos, 5.0f, 0.0f, 16, ax::Color4F::GREEN);
                    _connectionLines->drawSolidCircle(holderPos, 5.0f, 0.0f, 16, ax::Color4F::GREEN);

                    // Store connection information
                    ConnectionInfo connInfo;
                    connInfo.deck     = deck;
                    connInfo.holder   = holder;
                    connInfo.startPos = deckPos;
                    connInfo.endPos   = holderPos;
                    _connectionInfos.push_back(connInfo);
                }
            }
        }
        else
        {
            // Exit connect mode
            isConnectMode = false;

            // Clear connection lines and info
            if (_connectionLines)
            {
                _connectionLines->clear();
            }
            _connectionInfos.clear();

            // Clean up connect drag state
            _isConnectDragging = false;
            _connectSourceDeck = nullptr;
            if (_connectDragLine)
            {
                _connectDragLine->clear();
            }
        }
        break;
    case EventKeyboard::KeyCode::KEY_C:
        // Copy selected objects
        for (auto obj : _selectedObjects)
        {
            // Add to appropriate lists
            if (auto card = dynamic_cast<Card*>(obj))
            {
                Card *newCard = card->clone();
                newCard->setPosition(card->getPosition() + ax::Vec2(20, -20)); // Offset new card position
                this->addChild(newCard, CARD_ZORDER_BASE + _cards.size());
                _objects.push_back(newCard);
                _cards.push_back(newCard);
            }
            else if (auto deck = dynamic_cast<Deck*>(obj))
            {
                _decks.push_back(deck);
            }
            else if (auto rack = dynamic_cast<Rack*>(obj))
            {
                _racks.push_back(rack);
            }
            else if (auto table = dynamic_cast<Table*>(obj))
            {
                _tables.push_back(table);
            }
            else if (auto counter = dynamic_cast<Counter*>(obj))
            {
                _counters.push_back(counter);
            }
            
        }
        break;
    case EventKeyboard::KeyCode::KEY_M:
        // Exit zoom mode if active
        if (isZoomMode && _zoomedCard)
        {
            _zoomedCard->unzoom();
            isZoomMode  = false;
            _zoomedCard = nullptr;
        }

        // Exit connect mode if active
        if (isConnectMode)
        {
            isConnectMode = false;
            if (_connectionLines)
            {
                _connectionLines->clear();
            }
            _connectionInfos.clear();

            // Clean up connect drag state
            _isConnectDragging = false;
            _connectSourceDeck = nullptr;
            if (_connectDragLine)
            {
                _connectDragLine->clear();
            }
        }

        isMoveMode = !isMoveMode;
        clearSelectObjects();
        for (auto deck : _decks)
            deck->enableDragging(isMoveMode);
        for (auto rack : _racks)
            rack->enableDragging(isMoveMode);
        for (auto table : _tables)
            table->enableDragging(isMoveMode);
        for (auto counter : _counters)
            counter->enableDragging(isMoveMode);
        break;
    // case EventKeyboard::KeyCode::KEY_ESCAPE:
    //     Director::getInstance()->end();
    //     break;
    default:
        // Exit zoom mode on any other key
        if (isZoomMode && _zoomedCard)
        {
            _zoomedCard->unzoom();
            isZoomMode  = false;
            _zoomedCard = nullptr;
        }
        break;
    }
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    // Track Ctrl key state
    if (code == EventKeyboard::KeyCode::KEY_CTRL || code == EventKeyboard::KeyCode::KEY_LEFT_CTRL ||
        code == EventKeyboard::KeyCode::KEY_RIGHT_CTRL)
    {
        isCtrlPressed = false;
    }
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

    // Explicitly clean up game objects before scene destruction
    for (auto card : _cards)
    {
        if (card)
        {
            card->removeFromParent();
        }
    }
    _cards.clear();

    for (auto counter : _counters)
    {
        if (counter)
        {
            counter->removeFromParent();
        }
    }
    _counters.clear();

    for (auto rack : _racks)
    {
        if (rack)
        {
            rack->removeFromParent();
        }
    }
    _racks.clear();

    for (auto deck : _decks)
    {
        if (deck)
        {
            deck->removeFromParent();
        }
    }
    _decks.clear();

    for (auto table : _tables)
    {
        if (table)
        {
            table->removeFromParent();
        }
    }
    _tables.clear();

    _objects.clear();
    _selectedObjects.clear();
    _draggedObjects.clear();

    // Clean up zoom mode
    if (isZoomMode && _zoomedCard)
    {
        _zoomedCard->unzoom();
        isZoomMode  = false;
        _zoomedCard = nullptr;
    }

    // Clean up connection lines
    if (_connectionLines)
    {
        _connectionLines->removeFromParent();
        _connectionLines = nullptr;
    }

    // Clean up connect drag line
    if (_connectDragLine)
    {
        _connectDragLine->removeFromParent();
        _connectDragLine = nullptr;
    }

    if (_selectionRectangle)
    {
        _selectionRectangle->removeFromParent();
        _selectionRectangle = nullptr;
    }

    if (_addMenu)
    {
        _addMenu->removeFromParent();
        _addMenu = nullptr;
    }

    if (_addSpecificMenu)
    {
        _addSpecificMenu->removeFromParent();
        _addSpecificMenu = nullptr;
    }

    if (_touchListener)
        _eventDispatcher->removeEventListener(_touchListener);
    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
    _sceneID = -1;
}

DraggableObject* MainScene::getObjectAtPosition(const ax::Vec2& position, bool all)
{
    for (int i = _cards.size() - 1; i >= 0; i--)
    {
        auto card = _cards[i];
        if (card->containsPoint(position))
            return card;
    }
    if (!all)
        return nullptr;
    for (int i = _counters.size() - 1; i >= 0; i--)
    {
        auto counter = _counters[i];
        if (counter->containsPoint(position))
            return counter;
    }

    for (int i = _racks.size() - 1; i >= 0; i--)
    {
        auto rack = _racks[i];
        if (rack->containsPoint(position))
            return rack;
    }
    for (int i = _decks.size() - 1; i >= 0; i--)
    {
        auto deck = _decks[i];
        if (deck->containsPoint(position))
            return deck;
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

void MainScene::getAllObjects(std::vector<DraggableObject*>& outObjects)
{
    outObjects.clear();
    for (auto counter : _counters)
        outObjects.push_back(counter);
    for (auto card : _cards)
        outObjects.push_back(card);
    for (auto rack : _racks)
        outObjects.push_back(rack);
    for (auto deck : _decks)
        outObjects.push_back(deck);
    for (auto table : _tables)
        outObjects.push_back(table);
    sortObjectsByZOrder(outObjects);
}

void MainScene::createSpecificAddMenu(std::string folderName)
{
    if (_addSpecificMenu)
    {
        this->removeChild(_addSpecificMenu);
        _addSpecificMenu = nullptr;
    }

    // Check if folder has subfolders defined
    if (_subAddFolder.find(folderName) == _subAddFolder.end() || _subAddFolder[folderName].empty())
    {
        auto fileUtils       = ax::FileUtils::getInstance();
        vector<string> files = fileUtils->listFiles(folderName);

        // Filter out directories and keep only .png files
        vector<string> pngFiles;
        for (const auto& file : files)
        {
            if (file.find(".png") != string::npos || file.find(".jpg") != string::npos)
            {
                pngFiles.push_back(file);
            }
        }

        // If no png files, add default element directly
        if (pngFiles.empty())
        {
            if (folderName == "decks")
                addDeck("");
            else if (folderName == "racks")
                addRack("");
            else if (folderName == "tables")
                addTable("");
            else if (folderName == "counters")
                addCounter("");
            return;
        }

        // Create menu items for each file
        for (const auto& file : pngFiles)
        {
            vector<string> parts = split(file, '/');  // get filename from path
            string fileName      = parts.back();

            // Determine which add function to call based on folder
            if (folderName == "cards")
            {
                auto _addFolder = createMenuEntry(fileName, [=](auto) { addCard(folderName + "/" + fileName); });
                _addSpecificItems.pushBack(_addFolder);
            }
            else if (folderName.find("cards") == 0)  // cards/subfolder/
            {
                auto _addFolder = createMenuEntry(fileName, [=](auto) { addCard(file); });
                _addSpecificItems.pushBack(_addFolder);
            }
            else if (folderName == "racks")
            {
                auto _addFolder = createMenuEntry(fileName, [=](auto) { addRack(folderName + "/" + fileName); });
                _addSpecificItems.pushBack(_addFolder);
            }
            else if (folderName == "counters")
            {
                auto _addFolder = createMenuEntry(fileName, [=](auto) { addCounter(folderName + "/" + fileName); });
                _addSpecificItems.pushBack(_addFolder);
            }
        }
    }
    else
    {
        // Has subfolders, create menu for each
        for (const auto& subFolder : _subAddFolder[folderName])
        {
            auto _addFolder =
                createMenuEntry(subFolder, [=](auto) { createSpecificAddMenu(folderName + "/" + subFolder); });
            _addSpecificItems.pushBack(_addFolder);
        }
    }

    // Only create menu if there are items
    if (!_addSpecificItems.empty())
    {
        _addSpecificMenu = ax::Menu::createWithArray(_addSpecificItems);
        this->addChild(_addSpecificMenu, 10001);
        _addSpecificMenu->setPosition(Vec2(_addMenu->getPositionX(), _addMenu->getPositionY()));
        _addSpecificMenu->alignItemsVerticallyWithPadding(0);
        _addSpecificItems.clear();
    }
}

void MainScene::addCard(std::string cardName)
{
    auto card = Card::create(cardName, "cards/Card Back 1.png");
    if (card)
    {
        card->setPosition(Vec2(_addMenu->getPositionX(), _addMenu->getPositionY()));
        this->addChild(card, CARD_ZORDER_BASE + _cards.size());
        _cards.push_back(card);
    }
    else
    {
        problemLoading("card_front.png or card_back.png");
    }
}

void MainScene::addDeck(std::string deckName)
{
    // Deck doesn't use texture - create default gray deck
    auto deck = Deck::create();
    if (deck)
    {
        deck->setPosition(Vec2(_addMenu->getPositionX(), _addMenu->getPositionY()));
        this->addChild(deck, DECK_ZORDER_BASE + _decks.size());
        _decks.push_back(deck);
    }
    else
    {
        problemLoading("deck");
    }
}

void MainScene::addRack(std::string rackName)
{
    Rack* rack = nullptr;
    if (rackName.empty())
    {
        // No texture provided - need to handle default case
        // For now, skip or use a placeholder
        AXLOG("Warning: Rack requires a texture, none provided");
        return;
    }
    else
    {
        rack = Rack::create(rackName);
    }

    if (rack)
    {
        rack->setPosition(Vec2(_addMenu->getPositionX(), _addMenu->getPositionY()));
        this->addChild(rack, RACK_ZORDER_BASE + _racks.size());
        _racks.push_back(rack);
    }
    else
    {
        problemLoading(rackName.c_str());
    }
}

void MainScene::addTable(std::string tableName)
{
    // Table doesn't use texture - create default table
    auto table = Table::create();
    if (table)
    {
        table->setPosition(Vec2(_addMenu->getPositionX(), _addMenu->getPositionY()));
        auto deck = table->getDiscardDeck();
        this->addChild(table, TABLE_ZORDER_BASE + _tables.size());
        this->addChild(deck, DECK_ZORDER_BASE + _decks.size());
        _tables.push_back(table);
        _decks.push_back(deck);
        deck->setPosition(Vec2(table->getPositionX() - table->getContentSize().x / 2 - 50, table->getPositionY()));
    }
    else
    {
        problemLoading("table");
    }
}

void MainScene::addCounter(std::string counterName)
{
    Counter* counter = nullptr;
    if (counterName.empty())
    {
        // No texture provided
        AXLOG("Warning: Counter requires a texture, none provided");
        return;
    }
    else
    {
        counter = Counter::create(counterName);
    }

    if (counter)
    {
        counter->setPosition(Vec2(_addMenu->getPositionX(), _addMenu->getPositionY()));
        this->addChild(counter, COUNTER_ZORDER_BASE + _counters.size());
        _counters.push_back(counter);
    }
    else
    {
        problemLoading(counterName.c_str());
    }
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
            this->addChild(card, CARD_ZORDER_BASE + i);
            _cards.push_back(card);
        }
        else
        {
            problemLoading("card_front.png or card_back.png");
        }
        i++;
    }
}

void MainScene::loadRacks()
{
    _racks.push_back(Rack::create("racks/rack_blue.png"));
    _racks.push_back(Rack::create("racks/rack_red.png"));
    _racks[0]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + _racks[0]->getContentSize().y / 2));
    _racks[1]->setPosition(
        Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - _racks[1]->getContentSize().y / 2));
    for (size_t i = 0; i < _racks.size(); i++)
    {
        auto rack = _racks[i];
        if (!rack)
        {
            problemLoading("rack.png");
            continue;
        }
        this->addChild(rack, RACK_ZORDER_BASE + i);
    }
}

void MainScene::loadDecks()
{
    _decks.push_back(Deck::create());
    _decks[0]->setPosition(Vec2(origin.x + visibleSize.width - 100, origin.y + visibleSize.height / 2));
    for (size_t i = 0; i < _decks.size(); i++)
    {
        auto deck = _decks[i];
        if (!deck)
        {
            problemLoading("deck.png");
            continue;
        }
        deck->setConnectedHolders(castVector<Holder>(_racks));
        this->addChild(deck, DECK_ZORDER_BASE + i);
    }
}

void MainScene::loadTables()
{
    _tables.push_back(Table::create());
    _tables[0]->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    for (size_t i = 0; i < _tables.size(); i++)
    {
        auto table = _tables[i];
        if (!table)
        {
            problemLoading("table.png");
            continue;
        }
        auto deck = table->getDiscardDeck();
        this->addChild(table, TABLE_ZORDER_BASE + i);
        this->addChild(deck, DECK_ZORDER_BASE + _decks.size());
        _decks.push_back(deck);
        deck->setPosition(Vec2(table->getPositionX() - table->getContentSize().x / 2 - 50, table->getPositionY()));
    }
}

void MainScene::loadCountersFromDirectory()
{
    // Get all files in a folder
    auto fileUtils = ax::FileUtils::getInstance();
    vector<string> files;

    string folderPath = "counters/";
    files             = fileUtils->listFiles(folderPath);
    int numCards      = static_cast<int>(files.size());
    int i             = 0;
    for (const auto& file : files)
    {
        vector<string> parts = split(file, '/');  // get filename from path
        auto counter         = Counter::create(folderPath + parts.back());
        if (counter)
        {
            counter->setPosition(
                Vec2(origin.x + (i + 1) * visibleSize.width / (numCards + 1), origin.y + visibleSize.height / 2));
            this->addChild(counter, COUNTER_ZORDER_BASE + i);
            _counters.push_back(counter);
        }
        else
        {
            problemLoading("counter sprite");
        }
        i++;
    }
}

void MainScene::updateHoverStates(const ax::Vec2& mousePos)
{
    sortObjectsByZOrder(_cards);

    DraggableObject* previousHovered = _hoveredObject;
    _hoveredObject                   = getObjectAtPosition(mousePos, isMoveMode);

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

    for (auto obj : _objects)
    {
        if (!dynamic_cast<Card*>(obj) && !isMoveMode)
            continue;
        // Check if card's bounding box inside selection rectangle
        Rect objBox = obj->getBoundingBox();
        Rect selectionBox =
            Rect(min(_selectionStartPoint.x, currentPoint.x), min(_selectionStartPoint.y, currentPoint.y),
                 abs(currentPoint.x - _selectionStartPoint.x), abs(currentPoint.y - _selectionStartPoint.y));

        if (selectionBox.containsPoint(Vec2(objBox.getMinX(), objBox.getMinY())) &&
            selectionBox.containsPoint(Vec2(objBox.getMaxX(), objBox.getMinY())) &&
            selectionBox.containsPoint(Vec2(objBox.getMaxX(), objBox.getMaxY())) &&
            selectionBox.containsPoint(Vec2(objBox.getMinX(), objBox.getMaxY())))
        {
            obj->setHighlight(true);
            if (!contains(_selectedObjects, obj))
            {
                _selectedObjects.push_back(obj);
            }
        }
        else
        {
            // Deselect the card
            obj->setHighlight(false);
            if (contains(_selectedObjects, obj))
            {
                auto it = std::find(_selectedObjects.begin(), _selectedObjects.end(), obj);
                _selectedObjects.erase(it);
            }
        }
    }
}

MainScene::ConnectionInfo* MainScene::getConnectionAtPosition(const ax::Vec2& position, float threshold)
{
    for (auto& connInfo : _connectionInfos)
    {
        // Calculate distance from point to line segment
        ax::Vec2 lineVec  = connInfo.endPos - connInfo.startPos;
        ax::Vec2 pointVec = position - connInfo.startPos;

        float lineLength = lineVec.length();
        if (lineLength < 0.001f)
            continue;

        // Project point onto line
        float t = pointVec.dot(lineVec) / (lineLength * lineLength);
        t       = std::max(0.0f, std::min(1.0f, t));  // Clamp to line segment

        ax::Vec2 projection = connInfo.startPos + lineVec * t;
        float distance      = position.distance(projection);

        if (distance <= threshold)
        {
            return &connInfo;
        }
    }
    return nullptr;
}
