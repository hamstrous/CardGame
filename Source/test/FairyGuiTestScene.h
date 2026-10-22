#pragma once

#include "axmol.h"
#include "FairyGUI.h"

#include <map>
#include <sstream>

USING_NS_FGUI;

class FairyGuiTestScene : public ax::Scene
{
public:
    CREATE_FUNC(FairyGuiTestScene);
    bool init() override;
    void update(float delta) override;

    //// mouse
    //bool onMouseDown(ax::Event* event);
    //bool onMouseUp(ax::Event* event);
    //bool onMouseMove(ax::Event* event);
    //bool onMouseScroll(ax::Event* event);

    //bool onMoveModeMouseDown(ax::Event* event);
    //bool onMoveModeMouseUp(ax::Event* event);
    //bool onMoveModeMouseMove(ax::Event* event);

    //// Keyboard
    //void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    //void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    FairyGuiTestScene();
    ~FairyGuiTestScene() override;

private:
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    int _sceneID                                 = 0;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    GRoot* _groot;
    GComponent* _view;

    void onClose(EventContext* context);
};
