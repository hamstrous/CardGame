#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
#include "GuiDataItem.h"
USING_NS_AX_EXT;
using namespace std;

class InspectorDataItem : public GuiDataItem
{

public:
    enum Type
    {
        TITLE,
        DESCRIPTION,
        DRAG_FLOAT,
        SLIDER_FLOAT,
        SELECTABLE_BUTTONS,
        TEXT
    };

    void initTitle(string title)
    {
        type        = Type::TITLE;
        this->title = title;
    };
    void initDescription(string description)
    {
        type              = Type::DESCRIPTION;
        this->description = description;
    };
    void initDragFloat(const vector<string>& nameList, const vector<float*>& refList)
    {
        type = Type::DRAG_FLOAT;
        for (auto name : nameList)
        {
            this->nameList.push_back(name);
        }
        for (auto ref : refList)
        {
            this->refList.push_back(ref);
        }
    };
    void initSliderFloat();
    void initSelectableButtons();
    void initText();

    void drawTitle();
    void drawDescription();
    void drawDragFloat();
    void drawSliderFloat();
    void drawSelectableButtons();
    void drawText();

    void drawGuiItem() override;

    Type type;
    string title;
    string description;
    vector<float*> refList;
    vector<function<void(float)>> callableList;
    vector<string> nameList;
};
