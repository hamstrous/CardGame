#pragma once

#include "axmol.h"

class InspectorData;

class Inspector : public ax::Node
{
public:
    Inspector() = default;

    static Inspector* getInstance();
    static void destroyInstance();

    void drawFloatPointer(const std::vector<float*>& floatPointers, const char* label, float speed = 0.5f);

    void drawGui();

    void drawBasic(const std::vector<InspectorData>& inspectorDataList);

private:
    static Inspector* _instance;
};
