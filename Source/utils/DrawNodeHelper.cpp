#include "DrawNodeHelper.h"
#include "helper.h"

std::vector<ax::Vec2> DrawNodeHelper::getCubBezierRasterizedControlPoints(ax::Vec2 origin,
                                                                          ax::Vec2 control,
                                                                          ax::Vec2 destination,
                                                                          unsigned int segments)
{
    std::vector<ax::Vec2> rasterizedControlPoints(segments + 1);

    float t = 0.0f;
    for (unsigned int i = 0; i < segments; i++)
    {

        rasterizedControlPoints[i].x = powf(1.0f - t, 2.0f) * origin.x + 2.0f * (1.0f - t) * t * control.x + t * t * destination.x;
        rasterizedControlPoints[i].y = powf(1.0f - t, 2.0f) * origin.y + 2.0f * (1.0f - t) * t * control.y + t * t * destination.y;
        t += 1.0f / segments;
    }

    rasterizedControlPoints[segments].x = destination.x;
    rasterizedControlPoints[segments].y = destination.y;

    return rasterizedControlPoints;
}

void DrawNodeHelper::drawSolidCornerRect(ax::DrawNode* drawNode,
    ax::Size nodeSize,
    ax::Color4F fillColor,
    ax::Color4F borderColor,
    float thickness,
    float roundedCorner)
{
    // Use for rasterizing the curve to polygon
    const unsigned int segment = 10;

    roundedCorner = std::min(roundedCorner, 1.f);
    roundedCorner = std::max(roundedCorner, 0.f);

    float cornerRadius = std::min(nodeSize.width, nodeSize.height) / 2 * roundedCorner;

    ax::Vec2 sizeX = nodeSize * ax::Vec2::UNIT_X;
    ax::Vec2 sizeY = nodeSize * ax::Vec2::UNIT_Y;

    ax::Vec2 roundedCornerX = cornerRadius * ax::Vec2::UNIT_X;
    ax::Vec2 roundedCornerY = cornerRadius * ax::Vec2::UNIT_Y;


    std::vector<ax::Vec2> polygonVertices;
    std::vector<ax::Vec2> rasterizedControlPoints;

    rasterizedControlPoints = getCubBezierRasterizedControlPoints(roundedCornerX, ax::Vec2::ZERO, roundedCornerY, segment);
    for (auto rasterizedControlPoint : rasterizedControlPoints)
    {
        polygonVertices.push_back(rasterizedControlPoint);
    }

    rasterizedControlPoints = getCubBezierRasterizedControlPoints(sizeY - roundedCornerY, sizeY, sizeY + roundedCornerX, segment);
    for (auto rasterizedControlPoint : rasterizedControlPoints)
    {
        polygonVertices.push_back(rasterizedControlPoint);
    }

    rasterizedControlPoints = getCubBezierRasterizedControlPoints(nodeSize - roundedCornerX, nodeSize, nodeSize - roundedCornerY, segment);
    for (auto rasterizedControlPoint : rasterizedControlPoints)
    {
        polygonVertices.push_back(rasterizedControlPoint);
    }

    rasterizedControlPoints = getCubBezierRasterizedControlPoints(sizeX + roundedCornerY, sizeX, sizeX - roundedCornerX, segment);
    for (auto rasterizedControlPoint : rasterizedControlPoints)
    {
        polygonVertices.push_back(rasterizedControlPoint);
    }

    drawNode->drawSolidPolygon(polygonVertices.data(), polygonVertices.size(), fillColor, thickness, borderColor);

}
