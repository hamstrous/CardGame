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

    //_imGui->
    AXLOGD("TestScene::init");

    RapidJsonHelper helper;
    Document _document;

    helper.getDocumentFromFile("save/test.json", _document);
    //AXLOGD("{}", _document["_id"]);
    std::string jsonString = helper.convertDocumentToString(_document);
    AXLOGD("Converted JSON: {}", jsonString.c_str());
    return true;
}

void TestScene::update(float delta)
{
}

void TestScene::menuCloseCallback(ax::Object* sender) {}

TestScene::TestScene() {}

TestScene::~TestScene()
{}

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
    ImGui::Text("FPS=%.1f", 1.f / ImGui::GetIO().DeltaTime);

    static int clicked = 0;
    if (ImGui::Button("Button"))
        clicked++;
    if (clicked & 1)
    {
        ImGui::SameLine();
        ImGui::Text("Thanks for clicking me!");
    }
}
