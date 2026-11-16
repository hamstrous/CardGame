#include "TestScene.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "utils/helper.h"

#include "core/file/RapidJsonHelper.h"
#include "ImGui/ImGuiPresenter.h"

#include "core/ui/GuiManager.h"
#include "core/singleton/InputManager.h"
#include "core/singleton/LayerManager.h"
#include "core/object/Widget.h"

#include "core/event/EventMode.h"

#include "core/const/GameConstants.h"

#include <map>
#include <sstream>


USING_NS_AX_EXT;
using namespace rapidjson;
using namespace constant;
using namespace ax;
using namespace std;

bool TestScene::init()
{
    // super init
    if (!Scene::init())
    {
        return false;
    }

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _guiManager   = GuiManager::getInstance();
    _inputManager = InputManager::getInstance();
    _layerManager = LayerManager::getInstance();
    _imGui        = ImGuiPresenter::getInstance();

    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = AX_CALLBACK_2(TestScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(TestScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 10);

    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(TestScene::onMouseMove, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_mouseListener, 10);

    //RapidJsonHelper helper;
    //Document _document;

    //helper.getDocumentFromFile("save/test.json", _document);
    ////AXLOGD("{}", _document["_id"]);
    //std::string jsonString = helper.convertDocumentToString(_document);
    //AXLOGD("Converted JSON: {}", jsonString.c_str());

    Widget* widget = Widget::create();
    widget->setContentSize(Size(100, 100));
    widget->setPosition(visibleSize / 2);
    _layerManager->addWidgetToObjectLayer(widget);

    Widget* widget2 = Widget::create();
    widget2->setContentSize(Size(100, 100));
    widget2->setPosition(visibleSize / 2 - Vec2(50,50));
    _layerManager->addWidgetToObjectLayer(widget2);

    Widget* widget3 = Widget::create();
    widget3->setContentSize(Size(100, 100));
    widget3->setPosition(visibleSize / 2 + Vec2(50, 50));
    _layerManager->addWidgetToObjectLayer(widget3);

    Widget* widget4 = Widget::create();
    widget4->setContentSize(Size(100, 100));
    widget4->setPosition(visibleSize / 2 + Vec2(100, 100));
    _layerManager->addWidgetToObjectLayer(widget4);

    auto vectorWidget = {widget, widget2, widget3, widget4};
    for (auto& w : vectorWidget)
    {
        w->addFace("card/Card Back 1.png");
        w->addFace("card/Card Back 2.png");
        w->addFace("card/Card Back 3.png");
    }

    // Add singleton with no auto release
    // Parent coordinates align with world coordinates
    // Need for layer, event, transform
    this->addChild(_inputManager);
    this->addChild(_layerManager);

    auto camera = this->getDefaultCamera();
    camera->setZoom(2.0f);
    camera->setPosition(Vec2(100, 100));

    return true;
}

void TestScene::update(float delta)
{}

bool TestScene::onMouseMove(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());
    auto mouseWorldPos = helper::convertScreenPositionToWorld(this, mousePos);
    AXLOGD("Mouse position : ({};{})", mousePos.x, mousePos.y);
    AXLOGD("World position: ({};{})", mouseWorldPos.x, mouseWorldPos.y);
    return false;
}

void TestScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    if (code == EventKeyboard::KeyCode::KEY_E)
    {
        EventMode* e = new EventMode(Mode::EDIT);
        _eventDispatcher->dispatchEvent(e);
    }
    else if (code == EventKeyboard::KeyCode::KEY_P)
    {
        EventMode* e = new EventMode(Mode::PLAY);
        _eventDispatcher->dispatchEvent(e);
    }
    else if (code == EventKeyboard::KeyCode::KEY_S)
    {
        EventMode* e = new EventMode(Mode::SELECT);
        _eventDispatcher->dispatchEvent(e);
    }
    
}

void TestScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void TestScene::menuCloseCallback(Object* sender) {}

TestScene::TestScene() {}

TestScene::~TestScene()
{
    _inputManager->destroyInstance();
    _layerManager->destroyInstance();
    _guiManager->destroyInstance();
}

void TestScene::onEnter() {
    
    Scene::onEnter();
    ImGuiPresenter::getInstance()->addFont(R"(C:\Windows\Fonts\msyh.ttc)");

    ImGuiPresenter::getInstance()->enableDPIScale();  // enable dpi scale for 4K display support, depends at least
                                                        // one valid ttf/ttc font was added.
    ImGuiPresenter::getInstance()->addRenderLoop("#im01", AX_CALLBACK_0(TestScene::onImGuiDraw, this), this);
}

void TestScene::onExit()
{
    Scene::onExit();
    ImGuiPresenter::getInstance()->removeRenderLoop("#im01");
}

void TestScene::onImGuiDraw()
{
    //ImGui::Text("FPS=%.1f", 1.f / ImGui::GetIO().DeltaTime);

    //static int clicked = 0;
    //if (ImGui::Button("Button"))
    //    clicked++;
    //if (clicked & 1)
    //{
    //    ImGui::SameLine();
    //    ImGui::Text("Thanks for clicking me!");
    //}

    _guiManager->drawGui();
}
