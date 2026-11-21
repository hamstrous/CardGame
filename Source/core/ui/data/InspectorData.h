#pragma once

#include "axmol.h"

// For template
#include <vector>

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
    static bool sameIntPointers(const std::vector<int*>& intPointers, int*& outInt);
    static void setAllIntPointers(const std::vector<int*>& intPointers, int value);

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
    bool* isAllowClick = nullptr;

    // Allow movement
    bool* isAllowDrag = nullptr;

    // Allow rotation
    std::set<int>* allowedRotation; // 0->360
    std::function<void(int)> allowRotationCallback    = nullptr;
    std::function<void(int)> notAllowRotationCallback = nullptr;

    // Face cycle
    int* faceCycle = nullptr;

    // Enlarge
    int* enlargeRatio = nullptr;

    // HOLDER
    bool haveHolderData = false;  // flag
    static bool allHaveHolderData(const std::vector<InspectorData>& datas);

    // Spread
    ax::Vec2* spreadDirection = nullptr;



public:
    // Template
    template <typename T>
    static bool samePointers(const std::vector<T*>& pointers, T*& out)
    {
        bool samePointer = true;
        if (pointers.empty())
            return false;
        out = pointers[0];
        for (T* p : pointers)
        {
            if (p == nullptr)
                return false;
            if (*p != *out)
            {
                samePointer = false;
                break;
            }
        }
        return samePointer;
    }

    template <typename T>
    static void setAllPointers(std::vector<T*> pointers, T value)
    {
        for (auto& pointer : pointers)
        {
            *pointer = value;
        }
    }


};




