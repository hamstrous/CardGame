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

    void drawGui();

    void drawBasic(const std::vector<InspectorData>& inspectorDataList);

private:
    static Inspector* _instance;
};
