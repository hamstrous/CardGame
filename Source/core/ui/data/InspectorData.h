#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"

#include "InspectorDataItem.h"
#include "GuiData.h"

USING_NS_AX_EXT;
using namespace std;

class InspectorData : public GuiData
{

public:
    InspectorData() = default;

    void add(InspectorDataItem item) { itemList.push_back(item); }
    void drawGui() override;

    vector<InspectorDataItem> itemList;

};
