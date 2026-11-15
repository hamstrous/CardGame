#pragma once

#include "axmol.h"

#include "GuiData.h"

class InspectorData : public GuiData
{

public:
    // Helper
    static bool sameBool(const std::vector<bool>& bools, bool& outBool);

    static bool sameBoolPointers(const std::vector<bool*>& boolPointers, bool*& outBool);
    static void setAllBoolPointers(const std::vector<bool*>& boolPointers, const bool value);
    static bool sameFloatPointers(const std::vector<float*>& floatPointers, float*& outFloat);
    static void setAllFloatPointers(const std::vector<float*>& floatPointers, const float value);

public:
    InspectorData() = default;

    // BASIC
    bool haveBasicData = false; // flag
    static bool allHaveBasicData(const std::vector<InspectorData>& datas);

    // Transform
    ax::Vec2* position = nullptr;
    static bool gatherHaveSamePositionX(const std::vector<InspectorData>& datas, float*& outPositionX);
    static bool gatherHaveSamePositionY(const std::vector<InspectorData>& datas, float*& outPositionY);
    static void setPositionX(const std::vector<InspectorData>& datas, float positionX);
    static void setPositionY(const std::vector<InspectorData>& datas, float positionY);

    float* rotation    = nullptr;
    ax::Vec2* size     = nullptr;

    // Allow click
    bool* isAllowClick          = nullptr;

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
