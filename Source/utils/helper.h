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
