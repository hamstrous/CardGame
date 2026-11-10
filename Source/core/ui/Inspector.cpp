#include "Inspector.h"

#include "core/singleton/InputManager.h"

Inspector* Inspector::_instance = nullptr;

Inspector* Inspector::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new Inspector();
    }
    return _instance;
}

void Inspector::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }
}

void Inspector::drawUI() {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    InputManager* InputManager = InputManager::getInstance();
    if (InputManager)
    {
        auto selectedWidgets = InputManager->getSelectedWidgets();
        
        for (auto widget : selectedWidgets)
        {
            //widget->drawProperties();
            auto data = widget->getInspectorData();

            ImGui::PushID(widget->getId().c_str());
            widget->getInspectorData().drawGui();
            ImGui::PopID();

            widget->markTransformDirty();
        }
        
    }

    ImGui::End();
}
