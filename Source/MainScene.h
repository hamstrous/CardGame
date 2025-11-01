#pragma once

#include "axmol.h"
#include "Card.h"
#include "Table.h"
#include "DraggableObject.h"

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
    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    MainScene();
    ~MainScene() override;

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

    std::vector<DraggableObject*> _draggedObjects;

    std::vector<DraggableObject*> _selectedObjects;
    void selectObjectsClear()
    {
        for (auto& obj : _selectedObjects)
            obj->setHighlight(false);
        _selectedObjects.clear();
    }

    DraggableObject* _hoveredObject;
    std::vector<Card*> _cards;
    std::vector<Table*> _tables;

    void loadCardsFromDirectory();
    void loadTables();
    const std::string cardTypeFolder = "uno/";

    int _cardClickCount = 0;  // For z-ordering cards on click

    DraggableObject* getObjectAtPosition(const ax::Vec2& position);
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

    // Template functions for sorting objects
    template <typename T>
    void sortObjectsByZOrder(std::vector<T>& objects)
    {
        std::sort(objects.begin(), objects.end(), [](T a, T b) { return a->getLocalZOrder() < b->getLocalZOrder(); });
    }

    template <typename T>
    void sortObjectsByPosition(std::vector<T>& objects)
    {
        std::sort(objects.begin(), objects.end(), [](T a, T b) {
            ax::Vec2 posA = a->getPosition();
            ax::Vec2 posB = b->getPosition();
            if (posA.x != posB.x)
            {
                return posA.x < posB.x;
            }
            return posA.y < posB.y;
        });
    }
};
