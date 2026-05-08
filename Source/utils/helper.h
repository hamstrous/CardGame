#pragma once

#include <axmol.h>

#include <vector>
#include <string>

namespace helper
{

std::vector<std::string> split(const std::string& str, char delimiter = ' ');
std::string getTextFileContent(const std::string& filePath);
bool containPoint(const ax::Node* node, const ax::Vec2& worldPoint);
ax::Vec2 getNodePositionInWorldSpace(ax::Node* node);
bool isWorldPositionInNode(ax::Node* node, const ax::Vec2& worldPosition);

void moveNodeToFront(ax::Node* node);
void reparentPreserveTransform(ax::Node* child, ax::Node* newParent);

void setNewParentWithNoSideEffect(ax::Node* child, ax::Node* newParent);

void addChildToCurrentSceneWithNoSideEffect(ax::Node* child);

float getWorldRotation(ax::Node* node);
float getWorldRoationSkewX(ax::Node* node);
float getWorldRoationSkewY(ax::Node* node);

ax::Vec2 getWorldScale(ax::Node* node);

template <typename T, typename V>
ax::Vector<T> castToVectorOfType(ax::Vector<V> v)
{
    ax::Vector<T> result;
    for (auto& item : v)
    {
        T castedItem = dynamic_cast<T>(item);
        if (castedItem)
            result.pushBack(castedItem);
    }
    return result;
}

}  // namespace helper
