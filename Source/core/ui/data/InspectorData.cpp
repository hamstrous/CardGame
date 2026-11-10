#include "InspectorData.h"

#include "core/singleton/InputManager.h"

void InspectorData::drawGui() {
    for (auto item : itemList)
    {
        item.drawGuiItem();
    }
}
