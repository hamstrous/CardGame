#pragma once

#include "axmol.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "core/file/RapidJsonHelper.h"

#include "core/ui/Inspector.h"
#include "core/ui/SelectManager.h"
#include "core/object/Widget.h"

#include <map>
#include <sstream>

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace rapidjson;

class TestScene : public ax::Scene
{
public:
    CREATE_FUNC(TestScene);
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

    TestScene();
    ~TestScene() override;

    void onEnter() override;
    void onExit() override;

    void onImGuiDraw();

    void rapidjsonTest();

private:
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    int _sceneID                                 = 0;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    Inspector* _inspector = Inspector::getInstance();
    SelectManager* _selectManager = SelectManager::getInstance();

    ImGuiPresenter* _imGui = ImGuiPresenter::getInstance();   
};
