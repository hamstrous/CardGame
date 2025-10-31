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

    DraggableObject* _draggedObject;  // Currently dragged card
    DraggableObject* _hoveredObject;  // Card under mouse cursor
    std::vector<Card*> _cards;
    std::vector<Table*> _tables;

    void loadCardsFromDirectory();
    void loadTables();
    const std::string cardTypeFolder = "uno/";

    void sortCardsByZOrder();

    int _cardClickCount = 0; // For z-ordering cards on click

    DraggableObject* getObjectAtPosition(const ax::Vec2& position);
    void updateHoverStates(const ax::Vec2& mousePos);
};

