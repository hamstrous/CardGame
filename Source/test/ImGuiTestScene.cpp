#include "ImGuiTestScene.h"

using namespace ax;
USING_NS_AX_EXT;

bool ImGuiTestScene::init()
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

    return true;
}


void ImGuiTestScene::menuCloseCallback(ax::Object* sender) {}

ImGuiTestScene::ImGuiTestScene() {}

ImGuiTestScene::~ImGuiTestScene()
{}

void ImGuiTestScene::onEnter() {
    
    Scene::onEnter();
    ImGuiPresenter::getInstance()->addFont(R"(C:\Windows\Fonts\msyh.ttc)");

    ImGuiPresenter::getInstance()->enableDPIScale();  // enable dpi scale for 4K display support, depends at least
                                                        // one valid ttf/ttc font was added.
    ImGuiPresenter::getInstance()->addRenderLoop("#im01", AX_CALLBACK_0(ImGuiTestScene::onImGuiDraw, this), this);
}

void ImGuiTestScene::onExit()
{
    Scene::onExit();
    ImGuiPresenter::getInstance()->removeRenderLoop("#im01");
}

void ImGuiTestScene::onImGuiDraw()
{
    ImGui::Text("FPS=%.1f", 1.f / ImGui::GetIO().DeltaTime);

    ImGui::Button("Hello world");
}
