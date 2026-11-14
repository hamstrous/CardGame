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

bool InspectorData::sameFloatPointer(const std::vector<float*>& floatPointers, float*& outFloat)
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

void InspectorData::setAllFloatPointer(const std::vector<float*>& floatPointers, const float value) {
    for (auto& floatPointer : floatPointers)
    {
        *floatPointer = value;
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

bool InspectorData::gatherHaveSamePositionX(const std::vector<InspectorData>& datas, float*& outPositionX)
{
    std::vector<float*> positionXVector;
    for (auto& data : datas)
    {
        positionXVector.push_back(&data.position->x);
    }
    bool ret = sameFloatPointer(positionXVector, outPositionX);
    return ret;
}

bool InspectorData::gatherHaveSamePositionY(const std::vector<InspectorData>& datas, float*& outPositionY) {
    std::vector<float*> positionYVector;
    for (auto& data : datas)
    {
        positionYVector.push_back(&data.position->y);
    }
    bool ret = sameFloatPointer(positionYVector, outPositionY);
    return ret;
}

void InspectorData::setPositionX(const std::vector<InspectorData>& datas, float positionX) {
    std::vector<float*> positionXVector;
    for (auto& data : datas)
    {
        positionXVector.push_back(&data.position->x);
    }

    setAllFloatPointer(positionXVector, positionX);
}

void InspectorData::setPositionY(const std::vector<InspectorData>& datas, float positionY) {
    std::vector<float*> positionYVector;
    for (auto& data : datas)
    {
        positionYVector.push_back(&data.position->y);
    }

    setAllFloatPointer(positionYVector, positionY);
}
