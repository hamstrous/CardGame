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

void Inspector::pushStyleSelectedButton()
{
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Inspector::pushStyleNonSelectedButton()
{
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.f, 0.7f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Inspector::pushStyleUndeterminedButton()
{
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.f, 0.5f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.f, 0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Inspector::popStyleButton()
{
    ImGui::PopStyleColor(4);
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

void Inspector::drawBoolPointer(const std::vector<bool*>& boolPointers, const char* labelTrue, const char* labelFalse)
{
    bool* value;
    bool sameValue = InspectorData::sameBoolPointers(boolPointers, value);
    auto buttonSize = ImVec2(130, 40);

    bool trueClicked = false;
    bool falseClicked = false;

    ImGui::PushFont(NULL, 16.0f);

    if (sameValue)
    {
        if (*value)
        {
            pushStyleSelectedButton();
            ImGui::Button(labelTrue, buttonSize);
            popStyleButton();
            ImGui::SameLine();

            pushStyleNonSelectedButton();
            falseClicked = ImGui::Button(labelFalse, buttonSize);
            popStyleButton();
        }
        else
        {
            pushStyleNonSelectedButton();
            trueClicked = ImGui::Button(labelTrue, buttonSize);
            popStyleButton();
            ImGui::SameLine();

            pushStyleSelectedButton();
            ImGui::Button(labelFalse, buttonSize);
            popStyleButton();
        }
    }
    else
    {
        pushStyleUndeterminedButton();
        trueClicked = ImGui::Button(labelTrue, buttonSize);
        popStyleButton();
        ImGui::SameLine();

        pushStyleUndeterminedButton();
        falseClicked = ImGui::Button(labelFalse, buttonSize);
        popStyleButton();
        
    }

    if (trueClicked)
    {
        InspectorData::setAllBoolPointers(boolPointers, true);
    }
    else if (falseClicked)
    {
        InspectorData::setAllBoolPointers(boolPointers, false);
    }

    ImGui::PopFont();

}

void Inspector::drawAllowedRotation(const std::vector<std::set<int>*>& allowedRotationLists,
                                    const std::vector<std::function<void(float)>>& allowRotationCallbackList,
                                    const std::vector<std::function<void(float)>>& notAllowRotationCallbackList)
{
    vector<int> degreeList {
        0,
        45,
        90,
        135,
        180,
        225,
        270,
        315,
        360
    };

    auto buttonSize = ImVec2(60, 40);

    for (int degree : degreeList)
    {
        bool allHaveDegree    = true;
        bool allNotHaveDegree = true;
        for (const auto& allowedRotation : allowedRotationLists)
        {
            if (allowedRotation->find(degree) == allowedRotation->end())
            {
                allHaveDegree = false;
            }
            else
            {
                allNotHaveDegree = false;
            }
        }

        if (degree > 0)
        {
            ImGui::SameLine();
        }

        if (allHaveDegree)
        {
            pushStyleSelectedButton();
            if (ImGui::Button(std::to_string(degree).c_str(), buttonSize))
            {
                for (const auto& notAllowRotationCallback : notAllowRotationCallbackList)
                {
                    notAllowRotationCallback(degree);
                }
            };
            popStyleButton();
        }
        else if (allNotHaveDegree)
        {
            pushStyleNonSelectedButton();
            if (ImGui::Button(std::to_string(degree).c_str(), buttonSize))
            {
                for (const auto& allowRotationCallback : allowRotationCallbackList)
                {
                    allowRotationCallback(degree);
                }
            }
            popStyleButton();
        }
        else
        {
            pushStyleUndeterminedButton();
            if (ImGui::Button(std::to_string(degree).c_str(), buttonSize))
            {
                for (const auto& allowRotationCallback : allowRotationCallbackList)
                {
                    allowRotationCallback(degree);
                }
            }
            popStyleButton();
        }
    }
}

void Inspector::drawMultipleIntValueButton(const std::vector<int*>& intPointers,
                                           const std::vector<int> allowedValues,
                                           const std::vector<std::string> labels)
{
    AXASSERT(allowedValues.size() == labels.size(), "Inspector draw multiple int button: values and labels must have equal size");
    auto buttonSize = ImVec2(60, 40);
    int* value;
    bool sameInt = InspectorData::sameIntPointers(intPointers, value);
    if (sameInt)
    {
        for (int i = 0; i < allowedValues.size(); i++)
        {
            if (i > 0)
                ImGui::SameLine();

            int allowedValue = allowedValues[i];
            string label     = labels[i];
            if (*value == allowedValue)
            {
                pushStyleSelectedButton();
                ImGui::Button(label.c_str(), buttonSize);
                popStyleButton();
            }
            else
            {
                pushStyleNonSelectedButton();
                if (ImGui::Button(label.c_str(), buttonSize))
                {
                    InspectorData::setAllIntPointers(intPointers, allowedValue);
                }
                popStyleButton();
            }
        }
    }
    else
    {
        for (int i = 0; i < allowedValues.size(); i++)
        {
            if (i > 0)
                ImGui::SameLine();

            int allowedValue = allowedValues[i];
            string label     = labels[i];
            pushStyleUndeterminedButton();
            if (ImGui::Button(label.c_str(), buttonSize))
            {
                InspectorData::setAllIntPointers(intPointers, allowedValue);
            }
            popStyleButton();
            
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

    // Separating the data into different lists 
    vector<float*> positionXList, positionYList;
    vector<float*> rotationList;
    vector<float*> widthList, heightList;
    vector<bool*> isAllowClickList, isAllowDragList;
    vector<std::set<int>*> allowedRotationLists;
    vector<function<void(float)>> allowRotationCallbackList, notAllowRotationCallbackList;
    vector<int*> faceCycleList, enlargeRatioList;

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
        if (data.isAllowClick)
        {
            isAllowClickList.push_back(data.isAllowClick);
        }
        if (data.isAllowDrag)
        {
            isAllowDragList.push_back(data.isAllowDrag);
        }
        if (data.allowedRotation)
        {
            allowedRotationLists.push_back(data.allowedRotation);
        }
        if (data.allowRotationCallback)
        {
            allowRotationCallbackList.push_back(data.allowRotationCallback);
        }
        if (data.notAllowRotationCallback)
        {
            notAllowRotationCallbackList.push_back(data.notAllowRotationCallback);
        }
        if (data.faceCycle)
        {
            faceCycleList.push_back(data.faceCycle);
        }
        if (data.enlargeRatio)
        {
            enlargeRatioList.push_back(data.enlargeRatio);
        }

    }

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Position");
    ImGui::PopFont();
    ImGui::Text("Where the widget is located");

    drawFloatPointer(positionXList, "X", 0.5f);
    ImGui::SameLine();
    drawFloatPointer(positionYList, "Y", 0.5f);
    drawFloatPointer(rotationList, "Rotation", 0.5f);
    drawFloatPointer(widthList, "Width", 0.5f);
    ImGui::SameLine();
    drawFloatPointer(heightList, "Height", 0.5f);

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Allow Click");
    ImGui::PopFont();
    drawBoolPointer(isAllowClickList, "Allow Clicking", "Do Nothing##click");

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Allow Drag");
    ImGui::PopFont();
    drawBoolPointer(isAllowDragList, "Allow Dragging", "Do Nothing##drag");

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Rotation Constraints");
    ImGui::PopFont();

    ImGui::Text("Select angles objects can rotate to");

    drawAllowedRotation(allowedRotationLists, allowRotationCallbackList, notAllowRotationCallbackList);

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Face Cycle");
    ImGui::PopFont();
    vector<int> allowedFaceCycle = {1, -1, 0};
    vector<string> faceCycleLabel = {"Forward", "Backward", "Random"};
    drawMultipleIntValueButton(faceCycleList, allowedFaceCycle, faceCycleLabel);

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Enlarge Preview");
    ImGui::PopFont();
    vector<int> allowedEnlargeRatio = {0, 1, 2, 3};
    vector<string> enlargeLabel     = {"Nothing", "Small", "Medium", "Big"};
    drawMultipleIntValueButton(enlargeRatioList, allowedEnlargeRatio, enlargeLabel);

}

void Inspector::drawHolder(const std::vector<InspectorData>& inspectorDataList)
{
    bool gatheredHolderData = InspectorData::allHaveHolderData(inspectorDataList);
    if (!gatheredHolderData)
    {
        ImGui::Text("%d widgets selected", inspectorDataList.size());
        return;
    }

    vector<Vec2*> spreadDirectionList;

    for (const auto& data : inspectorDataList)
    {
        if (data.spreadDirection) {
            spreadDirectionList.push_back(data.spreadDirection);
        }
    }

    ImGui::PushFont(NULL, 18.0f);
    ImGui::Text("Spread Mode");
    ImGui::PopFont();
    ImGui::Text("Where the widget is located");

    vector<Vec2> allowedSpreadDirection = {Vec2(-1, 0), Vec2(1, 0), Vec2(0, 1), Vec2(0, -1)};
    vector<string> spreadLabel     = {"Left", "Right", "Top", "Down"};
    drawMultipleValueButton<Vec2>(spreadDirectionList, allowedSpreadDirection, spreadLabel);
}

void Inspector::drawGui() {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    InputManager* InputManager = InputManager::getInstance();
    if (InputManager)
    {
        auto& selectedWidgets = InputManager->getSelectedWidgets();
        vector<InspectorData> inspectorDataList;

        for (auto widget : selectedWidgets)
        {
            auto data = widget->getInspectorData();
            inspectorDataList.push_back(data);
        }
        ImGui::PushID("Inspector");

        drawBasic(inspectorDataList);
        drawHolder(inspectorDataList);
        for (auto widget : selectedWidgets)
            widget->markTransformDirty(); // Mark dirty after updating with inspector

        ImGui::PopID();
    }

    ImGui::End();
}
