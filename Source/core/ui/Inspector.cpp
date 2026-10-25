#include "Inspector.h"

#include "SelectManager.h"

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

    SelectManager* selectManager = SelectManager::getInstance();
    if (selectManager)
    {
        auto selectedWidgets = selectManager->getSelectedWidgets();
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
