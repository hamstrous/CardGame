#pragma once

#include <axmol.h>

#include <vector>
#include <string>

static std::vector<std::string> split(const std::string& str, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    std::string token;
    for (char ch : str)
    {
        if (ch == delimiter)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
        {
            token += ch;
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

static std::string getTextFileContent(const std::string& filePath)
{
    auto fileUtils = ax::FileUtils::getInstance();
    std::string content = fileUtils->getStringFromFile(filePath);
    return content;
}

static bool containPoint(const ax::Node* node, const ax::Vec2& worldPoint)
{
    ax::Vec2 localPoint = node->convertToNodeSpace(worldPoint);
    ax::Vec2 nodeSize   = node->getContentSize();
    ax::Rect bbox           = ax::Rect(-nodeSize.x / 2, -nodeSize.y / 2, nodeSize.x, nodeSize.y);
    return bbox.containsPoint(localPoint);
}

static ax::Vec2 getNodePositionInWorldSpace(ax::Node* node) {
    return node->getParent()->convertToWorldSpace(node->getPosition());
}

static bool isWorldPositionInNode(ax::Node* node, const ax::Vec2& worldPosition) {
    auto rect = node->getBoundingBox();
    return rect.containsPoint(node->getParent()->convertToNodeSpace(worldPosition));
}

static void moveNodeToFront(ax::Node* node) {
    ax::Node* parent = node->getParent();  // save parent first
    node->retain();  // prevent deallocation because reference count in this instant is only 1 from the scene graph
    node->removeFromParentAndCleanup(false);          
    parent->addChild(node); // bring to front    
    node->release(); // balance the retain, scene graph hold the only reference again
}

static void setNewParentWithNoEffect(ax::Node* child, ax::Node* newParent) {
    ax::Vec2 worldPosition = getNodePositionInWorldSpace(child);
    child->retain();
    child->removeFromParentAndCleanup(false);
    newParent->addChild(child);
    child->setPosition(newParent->convertToNodeSpace(worldPosition));
    child->release();
}

static void addChildToCurrentSceneWithNoEffect(ax::Node* child) {
    ax::Scene* currentScene = ax::Director::getInstance()->getRunningScene();
    setNewParentWithNoEffect(child, currentScene);
}

static float getWorldRotation(ax::Node* node) {
    float rotation = node->getRotation();
    ax::Node* parent = node->getParent();
    while (parent) {
        rotation += parent->getRotation();
        parent = parent->getParent();
    }
    return rotation;
}

static ax::Vec2 getWorldScale(ax::Node* node) {
    ax::Vec2 scale = ax::Vec2(node->getScaleX(), node->getScaleY());
    ax::Node* parent = node->getParent();
    while (parent) {
        scale.x *= parent->getScaleX();
        scale.y *= parent->getScaleY();
        parent = parent->getParent();
    }
    return scale;
}

template <typename T, typename V>
static ax::Vector<T> castToVectorOfType(ax::Vector<V> v) {
    ax::Vector<T> result;
    for (auto& item : v) {
        T castedItem = dynamic_cast<T>(item);
        if(castedItem) result.pushBack(castedItem);
    }
    return result;
}
