#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;

class InspectorData;

class Inspector : public ax::Node
{
public:
    Inspector() = default;

    static Inspector* getInstance();
    static void destroyInstance();

    void drawGui();

    void drawBasic(const std::vector<InspectorData>& inspectorDataList);

private:
    static Inspector* _instance;
};
