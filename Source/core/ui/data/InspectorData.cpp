#include "InspectorData.h"

#include "core/singleton/InputManager.h"

bool InspectorData::sameBool(const std::vector<bool>& bools, bool& outBool)
{
    bool sameBool = true;
    if (bools.empty())
        return false;
    outBool = bools[0];
    for (bool b : bools)
    {
        if (b != outBool)
        {
            sameBool = false;
            break;
        }
    }
    return sameBool;
}

bool InspectorData::sameBoolPointers(const std::vector<bool*>& boolPointers, bool*& outBool)
{
    bool sameBoolPointer = true;
    if (boolPointers.empty())
        return false;
    outBool = boolPointers[0];
    for (bool* b : boolPointers)
    {
        if (b == nullptr)
            return false;
        if (*b != *outBool)
        {
            sameBoolPointer = false;
            break;
        }
    }
    return sameBoolPointer;
}

void InspectorData::setAllBoolPointers(const std::vector<bool*>& boolPointers, const bool value)
{
    for (auto& boolPointer : boolPointers)
    {
        *boolPointer = value;
    }
}

bool InspectorData::sameFloatPointers(const std::vector<float*>& floatPointers, float*& outFloat)
{
    bool sameFloatPointer = true;
    if (floatPointers.empty())
        return false;
    outFloat = floatPointers[0];
    for (float* f : floatPointers)
    {
        if (f == nullptr)
            return false;
        if (*f != *outFloat)
        {
            sameFloatPointer = false;
            break;
        }
    }
    return sameFloatPointer;
}

void InspectorData::setAllFloatPointers(const std::vector<float*>& floatPointers, const float value) {
    for (auto& floatPointer : floatPointers)
    {
        *floatPointer = value;
    }
}

bool InspectorData::sameIntPointers(const std::vector<int*>& intPointers, int*& outInt)
{
    bool sameIntPointer = true;
    if (intPointers.empty())
        return false;
    outInt = intPointers[0];
    for (int* i : intPointers)
    {
        if (i == nullptr)
            return false;
        if (*i != *outInt)
        {
            sameIntPointer = false;
            break;
        }
    }
    return sameIntPointer;
}

void InspectorData::setAllIntPointers(const std::vector<int*>& intPointers, int value)
{
    for (auto& intPointer : intPointers)
    {
        *intPointer = value;
    }
}

bool InspectorData::allHaveBasicData(const std::vector<InspectorData>& datas)
{
    if (datas.empty())
        return false;

    for (auto& data : datas)
    {
        if (!data.haveBasicData)
            return false;
    }
    return true;
}

bool InspectorData::allHaveHolderData(const std::vector<InspectorData>& datas)
{
    if (datas.empty())
        return false;

    for (auto& data : datas)
    {
        if (!data.haveHolderData)
            return false;
    }
    return true;
}

bool InspectorData::gatherHaveSamePositionX(const std::vector<InspectorData>& datas, float*& outPositionX)
{
    std::vector<float*> positionXVector;
    for (auto& data : datas)
    {
        positionXVector.push_back(&data.position->x);
    }
    bool ret = sameFloatPointers(positionXVector, outPositionX);
    return ret;
}

bool InspectorData::gatherHaveSamePositionY(const std::vector<InspectorData>& datas, float*& outPositionY) {
    std::vector<float*> positionYVector;
    for (auto& data : datas)
    {
        positionYVector.push_back(&data.position->y);
    }
    bool ret = sameFloatPointers(positionYVector, outPositionY);
    return ret;
}

void InspectorData::setPositionX(const std::vector<InspectorData>& datas, float positionX) {
    std::vector<float*> positionXVector;
    for (auto& data : datas)
    {
        positionXVector.push_back(&data.position->x);
    }

    setAllFloatPointers(positionXVector, positionX);
}

void InspectorData::setPositionY(const std::vector<InspectorData>& datas, float positionY) {
    std::vector<float*> positionYVector;
    for (auto& data : datas)
    {
        positionYVector.push_back(&data.position->y);
    }

    setAllFloatPointers(positionYVector, positionY);
}
