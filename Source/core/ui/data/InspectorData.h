#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"

#include "InspectorDataItem.h"
USING_NS_AX_EXT;
using namespace std;

class InspectorData
{

public:
    void Add(InspectorDataItem item) { itemList.push_back(item); }

    vector<InspectorDataItem> itemList;

};
