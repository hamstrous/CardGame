#include "InspectorDataItem.h"

#include "core/singleton/InputManager.h"

void InspectorDataItem::drawTitle() {
    ImGui::PushFont(NULL, 18);
    ImGui::Text(title.c_str());
    ImGui::PopFont();
}

void InspectorDataItem::drawDescription() {
    ImGui::Text(description.c_str());
}

void InspectorDataItem::drawDragFloat() {
    AXASSERT(nameList.size() == refList.size(), "Drag Float item name and reference size is not equal");
    int count = nameList.size();
    ImGui::PushItemWidth(100);
    for (int i = 0; i < count; i++)
    {
        auto name = nameList[i];
        auto ref  = refList[i];
        if (i > 0)
            ImGui::SameLine();
        ImGui::DragFloat(name.c_str(), ref, 0.5f);
    }
    ImGui::PopItemWidth();
}

void InspectorDataItem::drawSliderFloat() {

}

void InspectorDataItem::drawSelectableButtons() {}

void InspectorDataItem::drawText() {}

void InspectorDataItem::drawGuiItem()
{
    switch (type)
    {
    case Type::TITLE:
        drawTitle();
        break;

    case Type::DESCRIPTION:
        drawDescription();
        break;

    case Type::DRAG_FLOAT:
        drawDragFloat();
        break;

    case Type::SLIDER_FLOAT:
        drawSliderFloat();
        break;

    case Type::SELECTABLE_BUTTONS:
        drawSelectableButtons();
        break;

    case Type::TEXT:
        drawText();
        break;
    }

}
