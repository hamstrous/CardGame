#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/event/EventListenerZone.h"

#include "core/rule/Rule.h"
#include "core/rule/LogicUnit.h"
#include "core/model/GameState.h"


class GameScene : public ax::Scene
{
public:
    bool init() override;
    void update(float delta) override;

    void setUpObjects();
    void setUpRule();

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

    // 
    void onEnter() override;

    ~GameScene() override;

protected:
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    int _sceneID                                 = 0;

    Rule* _rule = nullptr;
    std::vector<LogicUnit*> _flows;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    GameState* _gameState = nullptr; 

    //EventListenerZone* _cardEventListener = nullptr;
};
