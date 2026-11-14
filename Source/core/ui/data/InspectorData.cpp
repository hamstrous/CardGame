#include "InspectorData.h"

#include "core/singleton/InputManager.h"

bool InspectorData::gatherHaveBasicData(const std::vector<InspectorData>& datas)
{
    int count = 0;
    for (const auto& data : datas)
    {
        if (data.haveBasicData)
            count++;
    }

    return (count == datas.size() && count != 0);
}
