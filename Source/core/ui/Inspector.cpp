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

void Inspector::drawFloatPointer(const std::vector<float*>& floatPointers, const char* label, float speed) {
    float* value;
    bool sameValue = InspectorData::sameFloatPointers(floatPointers, value);
    if (sameValue)
    {
        if (ImGui::DragFloat(label, value, speed))
        {
            InspectorData::setAllFloatPointers(floatPointers, *value);
        }
    }
    else {
        static map<string, char*> strMap;
        char* str = strMap[label];
        if (!str)
        {
            str = new char[128];
            strcpy(str, "");
            strMap[label] = str;
        }
        if (ImGui::InputTextWithHint(label, "--", str, IM_ARRAYSIZE(str),
                                     ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue))
        {
            float newValue;
            try
            {
                newValue = stof(str);
                InspectorData::setAllFloatPointers(floatPointers, newValue);
                strcpy(str, "");
            }
            catch (const std::exception&)
            {}
            strcpy(str, "");
        }
        if (ImGui::IsItemDeactivated())
        {
            strcpy(str, "");
        }
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

    vector<float*> positionXList, positionYList;
    vector<float*> rotationList;
    vector<float*> widthList, heightList;

    for (const auto& data : inspectorDataList)
    {
        if (data.position)
        {
            positionXList.push_back(&data.position->x);
            positionYList.push_back(&data.position->y);
        }
        if (data.rotation)
        {
            rotationList.push_back(data.rotation);
        }
        if (data.size)
        {
            widthList.push_back(&data.size->width);
            heightList.push_back(&data.size->height);
        }
    }

    drawFloatPointer(positionXList, "X", 0.5f);
    ImGui::SameLine();
    drawFloatPointer(positionYList, "Y", 0.5f);
    drawFloatPointer(rotationList, "Rotation", 0.5f);
    drawFloatPointer(widthList, "Width", 0.5f);
    ImGui::SameLine();
    drawFloatPointer(heightList, "Height", 0.5f);

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
            auto data = widget->getInspectorData();
            inspectorDataList.push_back(data);
        }
        ImGui::PushID("Inspector");

        drawBasic(inspectorDataList);
        for (auto widget : selectedWidgets)
            widget->markTransformDirty(); // Mark dirty after updating with inspector

        ImGui::PopID();
    }

    ImGui::End();
}
