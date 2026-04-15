#pragma once

#include <string>

class CardData
{
public:
    std::string frontImagePath;
    std::string backImagePath;
    bool isFaceUp = true;
    bool isDraggable = true;
};
