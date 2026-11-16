#pragma once

#include "axmol.h"

class InspectorData;

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

    void drawGui();

    void drawBasic(const std::vector<InspectorData>& inspectorDataList);

private:
    static Inspector* _instance;
};
