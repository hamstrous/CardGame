#pragma once

#include <axmol.h>

#include <vector>
#include <string>

class DrawNodeHelper
{
private:
    static std::vector<ax::Vec2> getCubBezierRasterizedControlPoints(ax::Vec2 origin,
                                                          ax::Vec2 control,
                                                          ax::Vec2 destination,
                                                          unsigned int segments);

public:
    static void drawSolidCornerRect(ax::DrawNode* drawNode,
                               ax::Size nodeSize,
                               ax::Color4F fillColor,
                               ax::Color4F borderColor,
                               float thickness,
                               float roundedCorner); // between 0 and 1
};
