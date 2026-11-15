#include "ContextMenu.h"

#include "core/singleton/InputManager.h"

#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui.h"

#include "core/object/Widget.h"

#include "core/ui/data/InspectorData.h"

USING_NS_AX_EXT;
using namespace std;
using namespace ax;

ContextMenu* ContextMenu::_instance = nullptr;

ContextMenu* ContextMenu::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ContextMenu();
    }
    return _instance;
}

void ContextMenu::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }
}


void ContextMenu::drawGui() {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("ContextMenu", NULL, ImGuiWindowFlags_AlwaysAutoResize);


    ImGui::End();
}
