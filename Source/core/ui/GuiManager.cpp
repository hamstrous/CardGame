#include "GuiManager.h"

#include "core/singleton/InputManager.h"
#include "core/singleton/ModeManager.h"
#include "Inspector.h"
#include "ContextMenu.h"

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

void GuiManager::drawGui() {

    auto runningScene = ax::Director::getInstance()->getRunningScene();
    if (runningScene == nullptr)
        return;
    auto camera = runningScene->getDefaultCamera();

    static float cameraX      = camera->getPositionX();
    static float cameraY      = camera->getPositionY();
    static float cameraFactor = camera->getZoom();

    ImGui::Begin("Camera Test");

    if (ImGui::DragFloat("X", &cameraX, 0.5f))
    {
        camera->setPositionX(cameraX);
    }
    if (ImGui::DragFloat("Y", &cameraY, 0.5f))
    {
        camera->setPositionX(cameraY);
    }

    if (ImGui::DragFloat("Factor", &cameraFactor, 0.005f))
    {
        camera->setZoom(cameraFactor);
        camera->applyZoom();
    }
    ImGui::End();

    if (ModeManager::getInstance()->getCurrentMode() != Mode::EDIT)
        return;

    ContextMenu* contextMenu = ContextMenu::getInstance();
    contextMenu->drawGui();
    Inspector* inspector = Inspector::getInstance();
    inspector->drawGui();
}
