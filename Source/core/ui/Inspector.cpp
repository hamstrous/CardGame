#include "Inspector.h"

#include "core/singleton/InputManager.h"

#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui.h"

#include "core/object/Widget.h"

#include "core/ui/data/InspectorData.h"

USING_NS_AX_EXT;
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
    bool gatheredBasicData = InspectorData::allHaveBasicData(inspectorDataList);
    if (!gatheredBasicData)
    {
        ImGui::Text("%d widgets selected", inspectorDataList.size());
        return;
    }
    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Position");
    ImGui::PopFont();
    ImGui::Text("Where the widget is located");

    float *positionX, *positionY; // Representation of all positions
    bool sameX = InspectorData::gatherHaveSamePositionX(inspectorDataList, positionX);
    bool sameY = InspectorData::gatherHaveSamePositionY(inspectorDataList, positionY);

    // positionXY garantee to be equal to all position in list if that is the case

    if (sameX)
    {
        if (ImGui::DragFloat("X", positionX, 0.5f))
        {
            InspectorData::setPositionX(inspectorDataList, *positionX);
        }
    }
    else
    {
        static char str[128];
        if (ImGui::InputTextWithHint("X", "--", str, IM_ARRAYSIZE(str),
                                     ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            float value;
            try
            {
                value = stof(str);
                *positionX = value;
                InspectorData::setPositionX(inspectorDataList, *positionX);
                strcpy(str, "");
            }
            catch (const std::exception&)
            {
            }
            strcpy(str, "");
        }
        if (ImGui::IsItemDeactivated())
        {
            strcpy(str, "");
        }
    }


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
