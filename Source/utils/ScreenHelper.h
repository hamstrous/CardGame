#pragma once

#include <axmol.h>

#include <vector>
#include <string>

class ScreenHelper
{
public:
    static ax::Vec2 convertScreenPositionToWorld(ax::Vec2 screenPosition);
    static ax::Vec2 convertWorldPositionToScreen(ax::Vec2 worldPosition);

    static void getWorldPositionForEnlargePreview(ax::Node* node, ax::Sprite* enlargeSprite, int enlargeRatio);
};
