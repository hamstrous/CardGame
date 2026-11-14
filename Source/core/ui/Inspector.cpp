#include "Inspector.h"

#include "core/singleton/InputManager.h"

using namespace std;
using namespace ax;

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

void Inspector::drawBasic(const vector<InspectorData>& inspectorDataList)
{
    bool gatheredBasicData = InspectorData::gatherHaveBasicData(inspectorDataList);
    if (!gatheredBasicData)
    {
        ImGui::Text("%d widgets selected", inspectorDataList.size());
        return;
    }
    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Position");
    ImGui::PopFont();
    ImGui::Text("Where the widget is located");

    

}

void Inspector::drawGui() {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    InputManager* InputManager = InputManager::getInstance();
    if (InputManager)
    {
        auto selectedWidgets = InputManager->getSelectedWidgets();
        vector<InspectorData> inspectorDataList;

        for (auto widget : selectedWidgets)
        {
            //widget->drawProperties();
            auto data = widget->getInspectorData();
            inspectorDataList.push_back(data);

            

            widget->markTransformDirty();
        }
        ImGui::PushID("Inspector");
        drawBasic(inspectorDataList);
        ImGui::PopID();
    }

    ImGui::End();
}
