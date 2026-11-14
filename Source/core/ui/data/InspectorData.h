#pragma once

#include "axmol.h"

#include "GuiData.h"

class InspectorData : public GuiData
{

public:
    InspectorData() = default;

    // BASIC
    bool haveBasicData = false; // flag
    static bool gatherHaveBasicData(const std::vector<InspectorData>& datas)
    {
        int count = 0;
        for (const auto& data : datas)
        {
            if (data.haveBasicData)
                count++;
        }

        return (count == datas.size() && count != 0);
    }

    // Transform
    ax::Vec2* position = nullptr;
    float* rotation    = nullptr;
    ax::Vec2* size     = nullptr;

    // Allow click
    std::function<bool()> isAllowClick          = nullptr;
    std::function<void()> allowClickCallback    = nullptr;
    std::function<void()> notAllowClickCallback = nullptr;

    // Allow movement
    std::function<bool()> isAllowMovement          = nullptr;
    std::function<void()> allowMovementCallback    = nullptr;
    std::function<void()> notAllowMovementCallback = nullptr;

    // Allow rotation
    std::set<int>* allowedRotation;
    std::function<void(float)> allowRotationCallback    = nullptr;
    std::function<void(float)> notAllowRotationCallback = nullptr;

    // DECK
    bool haveDeckData = false;  // flag
    // Spread
    float* horizontalSpread = nullptr;
    float* verticalSpread   = nullptr;

};
