#include "Inspector.h"

#include "InputManager.h"

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
        if (!selectedWidgets.empty())
        {
            for (auto widget : selectedWidgets)
            {
                widget->drawProperties();  
            }
        }
    }

    ImGui::End();
}
