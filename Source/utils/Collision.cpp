#include "Collision.h"
#include "helper.h"

bool Collision::overlap(Polygon polygon1, Polygon polygon2, ax::Vec2& minimumTranslationVector)
{
    // Get all axis
    std::vector<Axis> axes1 = polygon1.getAxes();
    std::vector<Axis> axes2 = polygon2.getAxes();
    std::vector<Axis> axes;
    // Combine into one
    axes.reserve(axes1.size() + axes2.size());  // preallocate memory
    axes.insert(axes.end(), axes1.begin(), axes1.end());
    axes.insert(axes.end(), axes2.begin(), axes2.end());


    // for mtv
    float minimumOverlap = std::numeric_limits<float>::max();
    Axis minimumAxis = Axis(0, 0);

    // Loop over axes 
    for (auto& axis : axes)
    {
        // Project each polygon onto axis
        Projection projection1 = polygon1.project(axis);
        Projection projection2 = polygon2.project(axis);

        if (!projection1.overlap(projection2)) {
            // Once found an axis that not overlap then it is sure to not overlap
            return false;
        }
        else
        {
            // The minimum overlap with axis direction will be mtv
            float overlap = projection1.getOverlap(projection2);

            // If contain in other projection
            // Have to add the smallest different to mtv so it can get the polygon out
            if (projection1.contains(projection2) || projection2.contains(projection1))
            {
                float mins = abs(projection1.min - projection2.min);
                float maxs = abs(projection1.max - projection2.max);

                overlap += std::min(mins, maxs);
            }

            if (overlap < minimumOverlap)
            {
                overlap     = minimumOverlap;
                minimumAxis = axis;
            }
        }
    }

    minimumTranslationVector = minimumAxis * minimumOverlap;
    return true;
}

bool Collision::overlap(ax::Node* node1, ax::Node* node2, ax::Vec2& minimumTranslationVector)
{
    std::vector<ax::Vec2> node1Vectices, node2Vectices;

    // Convert node boundpoint to world space then create polygon
    node1Vectices.push_back(node1->convertToWorldSpace(ax::Vec2(0, 0)));
    node1Vectices.push_back(node1->convertToWorldSpace(node1->getContentSize()));
    node1Vectices.push_back(node1->convertToWorldSpace(ax::Vec2(node1->getContentSize().width, 0)));
    node1Vectices.push_back(node1->convertToWorldSpace(ax::Vec2(0, node1->getContentSize().height)));

    node2Vectices.push_back(node2->convertToWorldSpace(ax::Vec2(0, 0)));
    node2Vectices.push_back(node2->convertToWorldSpace(node2->getContentSize()));
    node2Vectices.push_back(node2->convertToWorldSpace(ax::Vec2(node2->getContentSize().width, 0)));
    node2Vectices.push_back(node2->convertToWorldSpace(ax::Vec2(0, node2->getContentSize().height)));

    Polygon polygon1(node1Vectices);
    Polygon polygon2(node2Vectices);

    return overlap(polygon1, polygon2, minimumTranslationVector);
}

bool Collision::overlap(ax::Node* node, ax::Vec2 worldPoint)
{
    // Convert worldPoint to node space and check if it in node bound box
    ax::Vec2 nodePoint = node->convertToNodeSpace(worldPoint);
    ax::Size nodeSize  = node->getContentSize();
    return (nodePoint.x <= nodeSize.width && nodePoint.x >= 0) && (nodePoint.y <= nodeSize.height && nodePoint.y >= 0);
}
