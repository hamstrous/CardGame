#pragma once

#include <string>

class ZoneData
{
public:
    std::string frontImagePath;
    std::string backImagePath;
    bool isFaceUp = true;
    bool isDraggable = true;
};
