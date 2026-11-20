#pragma once

#include "axmol.h"

#include <vector>
#include <string>

#include "ImGui/ImGuiPresenter.h"

#include "data/InspectorData.h"

class Inspector : public ax::Node
{
public:
    Inspector() = default;

    static Inspector* getInstance();
    static void destroyInstance();

    void pushStyleSelectedButton();
    void pushStyleNonSelectedButton();
    void pushStyleUndeterminedButton();

    void popStyleButton();

    void drawFloatPointer(const std::vector<float*>& floatPointers, const char* label, float speed = 0.5f);
    void drawBoolPointer(const std::vector<bool*>& boolPointers, const char* labelTrue, const char* labelFalse);

    void drawAllowedRotation(const std::vector<std::set<int>*>& allowedRotationLists,
                             const std::vector<std::function<void(float)>>& allowRotationCallbackList,
                             const std::vector<std::function<void(float)>>& notAllowRotationCallbackList);

    void drawMultipleIntValueButton(const std::vector<int*>& intPointers, const std::vector<int> allowedValues, const std::vector<std::string> labels);

    template <typename T>
    void drawMultipleValueButton(const std::vector<T*>& valuePointers,
                                               const std::vector<T> allowedValues,
                                               const std::vector<std::string> labels)
    {
        AXASSERT(allowedValues.size() == labels.size(),
                 "Inspector draw multiple value button: values and labels must have equal size");
        auto buttonSize = ImVec2(60, 40);
        T* value;
        bool sameValue = InspectorData::samePointers<T>(valuePointers, value);
        if (sameValue)
        {
            for (int i = 0; i < allowedValues.size(); i++)
            {
                if (i > 0)
                    ImGui::SameLine();

                T allowedValue = allowedValues[i];
                std::string label     = labels[i];
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
                        InspectorData::setAllPointers<T>(valuePointers, allowedValue);
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

                T allowedValue = allowedValues[i];
                std::string label     = labels[i];
                pushStyleUndeterminedButton();
                if (ImGui::Button(label.c_str(), buttonSize))
                {
                    InspectorData::setAllPointers<T>(valuePointers, allowedValue);
                }
                popStyleButton();
            }
        }
    }

    void drawGui();

    void drawBasic(const std::vector<InspectorData>& inspectorDataList);

    void drawHolder(const std::vector<InspectorData>& inspectorDataList);

private:
    static Inspector* _instance;
};
