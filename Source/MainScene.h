#pragma once

#include "axmol.h"
#include "Card.h"
#include "Rack.h"
#include "Deck.h"
#include "Table.h"
#include "DraggableObject.h"
#include "Counter.h"

class MainScene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };

public:
    bool init() override;
    void update(float delta) override;

    // mouse
    bool onMouseDown(ax::Event* event);
    bool onMouseUp(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseScroll(ax::Event* event);

    bool onMoveModeMouseDown(ax::Event* event);
    bool onMoveModeMouseUp(ax::Event* event);
    bool onMoveModeMouseMove(ax::Event* event);

    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    MainScene();
    ~MainScene() override;

    const int CARD_ZORDER_BASE  = 1000;
    const int COUNTER_ZORDER_BASE  = 1500;
    const int RACK_ZORDER_BASE  = -500;
    const int DECK_ZORDER_BASE  = -200;
    const int TABLE_ZORDER_BASE = -1000;

private:
    GameState _gameState                            = GameState::init;
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;
    int _sceneID                                    = 0;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    bool isMoveMode = false;

    std::vector<DraggableObject*> _draggedObjects;

    std::vector<DraggableObject*> _selectedObjects;
    void clearSelectObjects()
    {
        for (auto& obj : _selectedObjects)
            obj->setHighlight(false);
        _selectedObjects.clear();
    }

    DraggableObject* _hoveredObject;
    std::vector<DraggableObject*> _objects;
    std::vector<Card*> _cards;
    std::vector<Rack*> _racks;
    std::vector<Deck*> _decks;
    std::vector<Table*> _tables;
    std::vector<Counter*> _counters;
    void getAllObjects(std::vector<DraggableObject*>& outObjects);

    ax::Menu * _addMenu = nullptr;
    ax::Vector<ax::MenuItem*> _addMenuItems;

    void loadCardsFromDirectory();
    void loadRacks();
    void loadDecks();
    void loadTables();
    void loadCountersFromDirectory();
    const std::string cardTypeFolder = "uno/";

    DraggableObject* getObjectAtPosition(const ax::Vec2& position, bool all = false);
    void updateHoverStates(const ax::Vec2& mousePos);

    ax::DrawNode* _selectionRectangle = nullptr;
    ax::Vec2 _selectionStartPoint;
    void updateSelectionRectangle(const ax::Vec2& currentPoint);

    // Helper function to initialize dragging for an object
    bool initDrag(DraggableObject* obj, const ax::Vec2& mousePos);

    // Helper function to check if a value exists in a vector
    template <typename T, typename U>
    bool contains(const std::vector<T>& vec, const U& value)
    {
        return std::find(vec.begin(), vec.end(), value) != vec.end();
    }

    bool pushToTop(DraggableObject* obj)
    {
        if (auto card = dynamic_cast<Card*>(obj))
        {
            auto it = std::find(_cards.begin(), _cards.end(), card);
            if (it != _cards.end())
            {
                int index = std::distance(_cards.begin(), it);
                return pushToTop(_cards, index, CARD_ZORDER_BASE);
            }
        }
        else if (auto rack = dynamic_cast<Rack*>(obj))
        {
            auto it = std::find(_racks.begin(), _racks.end(), rack);
            if (it != _racks.end())
            {
                int index = std::distance(_racks.begin(), it);
                return pushToTop(_racks, index, RACK_ZORDER_BASE);
            }
        }
        else if (auto deck = dynamic_cast<Deck*>(obj))
        {
            auto it = std::find(_decks.begin(), _decks.end(), deck);
            if (it != _decks.end())
            {
                int index = std::distance(_decks.begin(), it);
                return pushToTop(_decks, index, DECK_ZORDER_BASE);
            }
        }
        else if (auto table = dynamic_cast<Table*>(obj))
        {
            auto it = std::find(_tables.begin(), _tables.end(), table);
            if (it != _tables.end())
            {
                int index = std::distance(_tables.begin(), it);
                return pushToTop(_tables, index, TABLE_ZORDER_BASE);
            }
        }

        return false;
    }

    template <typename T>
    bool pushToTop(std::vector<T>& vec, int index, int base)
    {
        if (index < 0 || index >= static_cast<int>(vec.size()))
            return false;

        T obj = vec[index];
        vec.erase(vec.begin() + index);
        vec.push_back(obj);
        for (size_t i = 0; i < vec.size(); ++i)
            vec[i]->setLocalZOrder(base + static_cast<int>(i));

        return true;
    }

    void filterObjects(std::vector<DraggableObject*>& objects,
                       std::vector<Card*>& cards,
                       std::vector<Rack*>& racks,
                       std::vector<Deck*>& decks,
                       std::vector<Table*>& tables);

    // Template functions for sorting objects
    template <typename T>
    void sortObjectsByZOrder(std::vector<T>& objects)
    {
        std::sort(objects.begin(), objects.end(), [](T a, T b) { return a->getLocalZOrder() < b->getLocalZOrder(); });
    }
};
