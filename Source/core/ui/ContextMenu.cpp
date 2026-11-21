#include "ContextMenu.h"

#include "core/singleton/InputManager.h"
#include "core/singleton/LayerManager.h"

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

void ContextMenu::justRightClicked()
{
    _justRightClicked = true;
}


void ContextMenu::drawGui() {
    InputManager* InputManager = InputManager::getInstance();
    if (InputManager)
    {
        auto& selectedWidgets = InputManager->getSelectedWidgets();
        ImGui::PushID("ContextMenu");
        const char* functionalities[] = {"Duplicate", "Delete"};
        if (_justRightClicked && !selectedWidgets.empty())
        {
            ImGui::OpenPopup("context_menu");
        }
        _justRightClicked = false;
        if (ImGui::BeginPopup("context_menu"))
        {
            ImGui::SeparatorText("Context Menu");
            for (int i = 0; i < IM_ARRAYSIZE(functionalities); i++)
                if (ImGui::Selectable(functionalities[i]))
                {
                    if (strcmp(functionalities[i], "Duplicate") == 0)
                    {
                        LayerManager::getInstance()->duplicateWidgets(selectedWidgets);
                    }
                    else if (strcmp(functionalities[i], "Delete") == 0)
                    {
                        LayerManager::getInstance()->deleteWidgets(selectedWidgets);
                    }
                }

            if (ImGui::BeginMenu("Layer"))
            {
                if(ImGui::MenuItem("Bring to front"))
                {
                    LayerManager::getInstance()->moveWidgetsToFront(selectedWidgets);
                }
                if(ImGui::MenuItem("Bring forward"))
                {
                    LayerManager::getInstance()->moveWidgetsForward(selectedWidgets);
                }
                if(ImGui::MenuItem("Bring to back"))
                {
                    LayerManager::getInstance()->moveWidgetsToBack(selectedWidgets);
                }
                if(ImGui::MenuItem("Bring backward"))
                {
                    LayerManager::getInstance()->moveWidgetsBackward(selectedWidgets);
                }
                ImGui::EndMenu();
            }
            
            ImGui::EndPopup();
        }

        ImGui::PopID();
    }

}
