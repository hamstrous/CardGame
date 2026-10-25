#include "TestScene.h"

using namespace ax;
using namespace rapidjson;
USING_NS_AX_EXT;

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

    //RapidJSON
    AXLOGD("TestScene::init");

    RapidJsonHelper helper;
    Document _document;

    helper.getDocumentFromFile("save/test.json", _document);
    //AXLOGD("{}", _document["_id"]);
    std::string jsonString = helper.convertDocumentToString(_document);
    AXLOGD("Converted JSON: {}", jsonString.c_str());

    Widget* widget = Widget::create();
    widget->setSizeAndDraw(ax::Size(100, 100));
    widget->setPosition(visibleSize / 2);
    this->addChild(widget);

    Widget* widget2 = Widget::create();
    widget2->setSizeAndDraw(ax::Size(100, 100));
    widget2->setPosition(visibleSize / 2 - Vec2(50,50));
    this->addChild(widget2);

    // Add singleton with no auto release
    // Parent coordinates align with world coordinates
    this->addChild(_inspector);
    this->addChild(_selectManager);

    _selectManager->addSelectedWidget(widget);
    //_selectManager->addSelectedWidget(widget2);

    return true;
}

void TestScene::update(float delta)
{
}

void TestScene::menuCloseCallback(ax::Object* sender) {}

TestScene::TestScene() {}

TestScene::~TestScene()
{
    _inspector->destroyInstance();
    _selectManager->destroyInstance();
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

    _inspector->drawUI();
}
