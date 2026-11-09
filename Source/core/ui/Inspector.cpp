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
    ImGui::Begin("Inspector");

    InputManager* InputManager = InputManager::getInstance();
    if (InputManager)
    {
        auto selectedWidgets = InputManager->getSelectedWidgets();
        
        for (auto widget : selectedWidgets)
        {
            //widget->drawProperties();
            auto data = widget->getInspectorData();

            ImGui::PushID(widget->getId().c_str());
            for (auto item : data.itemList)
            {
                ImGui::Text(item.name.c_str());
                for (auto itemitem : item.list)
                {
                    ImGui::DragFloat(itemitem.name.c_str(), itemitem.ref, 0.5f);
                }
            }

            ImGui::PopID();

        }
        
    }

    ImGui::End();
}
