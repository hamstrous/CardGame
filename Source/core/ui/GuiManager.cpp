#include "GuiManager.h"

#include "core/singleton/InputManager.h"
#include "ImGui/ImGuiPresenter.h"

GuiManager* GuiManager::_instance = nullptr;

GuiManager* GuiManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new GuiManager();
    }
    return _instance;
}

void GuiManager::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }
}

void GuiManager::drawUI() {
    ImGui::Begin("GuiManager");

    InputManager* InputManager = InputManager::getInstance();

    ImGui::End();
}
