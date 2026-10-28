#include "helper.h"

namespace helper
{

std::vector<std::string> split(const std::string& str, char delimiter)
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

std::string getTextFileContent(const std::string& filePath)
{
    auto fileUtils      = ax::FileUtils::getInstance();
    std::string content = fileUtils->getStringFromFile(filePath);
    return content;
}

std::vector<std::string> getFileNamesInFolder(const std::string& folderPath)
{
    std::vector<std::string> fileNames;

    auto fileUtils = ax::FileUtils::getInstance();
    auto files     = fileUtils->listFiles(folderPath);

    for (const auto& file : files)
    {
        std::vector<std::string> parts = split(file, '/');  // get filename from path
        fileNames.push_back(parts.back());                  // extract filename and add to list
    }

    return fileNames;
}

bool containPoint(const ax::Node* node, const ax::Vec2& worldPoint)
{
    ax::Vec2 localPoint = node->convertToNodeSpace(worldPoint);
    ax::Vec2 nodeSize   = node->getContentSize();
    ax::Rect bbox       = ax::Rect(-nodeSize.x / 2, -nodeSize.y / 2, nodeSize.x, nodeSize.y);
    return bbox.containsPoint(localPoint);
}

ax::Vec2 getNodePositionInWorldSpace(ax::Node* node)
{
    return node->getParent()->convertToWorldSpace(node->getPosition());
}

bool isWorldPositionInNode(ax::Node* node, const ax::Vec2& worldPosition)
{
    auto rect = node->getBoundingBox();  // aabb is in parent space
    return rect.containsPoint(node->getParent()->convertToNodeSpace(
        worldPosition));  // convert world position to parent space and check if it is in the bounding box
}

void moveNodeToFront(ax::Node* node)
{
    ax::Node* parent = node->getParent();  // save parent first
    node->retain();  // prevent deallocation because reference count in this instant is only 1 from the scene graph
    node->removeFromParentAndCleanup(false);
    parent->addChild(node);  // bring to front
    node->release();         // balance the retain, scene graph hold the only reference again
}

void setNewParentWithNoSideEffect(ax::Node* child, ax::Node* newParent)
{
    //// retrieve the node's node-to-world transform before changing parent
    // auto worldTx = child->getNodeToWorldTransform();

    //// ... perform your changing parent here
    // child->retain();
    // child->removeFromParentAndCleanup(false);
    // newParent->addChild(child);
    // child->release();

    //// the product of ancestor nodes' parent-to-node transforms
    // auto productTx = ax::Mat4::IDENTITY;

    //// enumerate upward through the hierarchy
    // for (auto parent = child->getParent(); parent != nullptr; parent = parent->getParent())
    //{
    //     // matrix multiplication
    //     productTx *= parent->getParentToNodeTransform();
    // }

    //// deduce the node's node-to-parent transform after changing parent
    // ax::Mat4 newNodeToParentTx = productTx * worldTx;
    // child->setNodeToParentTransform(newNodeToParentTx);

    // ax::Vec3 newScale;
    // ax::Quaternion newRotation;
    // ax::Vec3 newTranslation;

    // newNodeToParentTx.decompose(&newScale, &newRotation, &newTranslation);

    // child->setScaleX(newScale.x);
    // child->setScaleY(newScale.y);
    // child->setScaleZ(newScale.z);

    // child->setRotationQuat(newRotation);

    // child->setPosition(ax::Vec2(newTranslation.x, newTranslation.y));
    // child->setPositionZ(newTranslation.z);

    ax::Vec2 worldPosition = getNodePositionInWorldSpace(child);
    child->retain();
    // When an object change parent, the transform values stay the same, but relatively to the parent it change, so
    // this is for reverting that then use action to animate it smoothly
    child->setRotation(getWorldRotation(child) -
                       getWorldRotation(newParent));  // To get the absolute difference in rotation between the card and
                                                      // the zone and rotate it accordingly
    // child->setRotationSkewX(getWorldRoationSkewX(child) - getWorldRoationSkewX(newParent));  // To get the absolute
    // difference in rotation between the card and the zone and rotate it accordingly
    // child->setRotationSkewY(getWorldRoationSkewY(child) - getWorldRoationSkewY(newParent));  // To get the absolute
    // difference in rotation between the card and the zone and rotate it accordingly
    ax::Vec2 childScale  = getWorldScale(child);
    ax::Vec2 parentScale = getWorldScale(newParent);
    child->setScaleX(
        childScale.x /
        parentScale.x);  // To get the absolute difference in scale between the card and the zone and scale it
                         // accordingly child->setScaleY(childScale.y / parentScale.y);  // To get the absolute
                         // difference in scale between the card and the zone and scale it accordingly
    child->setScaleY(childScale.y / parentScale.y);

    child->removeFromParentAndCleanup(false);
    newParent->addChild(child);
    child->setPosition(newParent->convertToNodeSpace(worldPosition));
    child->release();
}

void addChildToCurrentSceneWithNoSideEffect(ax::Node* child)
{
    ax::Scene* currentScene = ax::Director::getInstance()->getRunningScene();
    setNewParentWithNoSideEffect(child, currentScene);
}

float getWorldRotation(ax::Node* node)
{
    float rotation   = node->getRotation();
    ax::Node* parent = node->getParent();
    while (parent)
    {
        rotation += parent->getRotation();
        parent = parent->getParent();
    }
    return rotation;
}

float getWorldRoationSkewX(ax::Node* node)
{
    float rotationSkewX = node->getRotationSkewX();
    ax::Node* parent    = node->getParent();
    while (parent)
    {
        rotationSkewX += parent->getRotationSkewX();
        parent = parent->getParent();
    }
    return rotationSkewX;
}

float getWorldRoationSkewY(ax::Node* node)
{
    float rotationSkewY = node->getRotationSkewY();
    ax::Node* parent    = node->getParent();
    while (parent)
    {
        rotationSkewY += parent->getRotationSkewY();
        parent = parent->getParent();
    }
    return rotationSkewY;
}

ax::Vec2 getWorldScale(ax::Node* node)
{
    ax::Vec2 scale   = ax::Vec2(node->getScaleX(), node->getScaleY());
    ax::Node* parent = node->getParent();
    while (parent)
    {
        scale.x *= parent->getScaleX();
        scale.y *= parent->getScaleY();
        parent = parent->getParent();
    }
    return scale;
}

ax::Rect getRectFromPoints(const ax::Vec2 pointA, const ax::Vec2 pointB)
{
    float minX = std::min(pointA.x, pointB.x);
    float minY = std::min(pointA.y, pointB.y);
    float maxX = std::max(pointA.x, pointB.x);
    float maxY = std::max(pointA.y, pointB.y);
    return ax::Rect(minX, minY, maxX - minX, maxY - minY);
} 

void setNodeAnchorPointPreservePosition(ax::Node* node, const ax::Vec2& newAnchorPoint) {
    ax::Vec2 oldAnchorPointInPoints = node->getAnchorPointInPoints();
    ax::Vec2 newAnchorPointInPoints = ax::Vec2(node->getContentSize().width * newAnchorPoint.x, node->getContentSize().height * newAnchorPoint.y);
    node->setAnchorPoint(newAnchorPoint);

    ax::Vec2 positionDelta = newAnchorPointInPoints - oldAnchorPointInPoints;
    positionDelta.rotate(ax::Vec2::ZERO, AX_DEGREES_TO_RADIANS(-node->getRotation()));
    node->setPosition(node->getPosition() + positionDelta);
}

ax::Vec3 getLineEquationFromTwoPoints(const ax::Vec2& pointA, const ax::Vec2& pointB)
{
    // use general form of line equation: Ax + By + C = 0
    float A = pointB.y - pointA.y;
    float B = pointA.x - pointB.x;
    float C = pointB.x * pointA.y - pointA.x * pointB.y;
    return ax::Vec3(A, B, C);
}

ax::Vec2 getSolutionForLineIntersection(const ax::Vec3& line1, const ax::Vec3& line2)
{
    // Solve the system of linear equations:
    // line1.A * x + line1.B * y + line1.C = 0
    // line2.A * x + line2.B * y + line2.C = 0
    float det = line1.x * line2.y - line2.x * line1.y;
    if (det == 0)
    {
        // Lines are parallel
        return ax::Vec2(0, 0);
    }
    // Cramer/determinant method to find the intersection point
    float x = (line2.z * line1.y - line1.z * line2.y) / det;
    float y = (line2.x * line1.z - line1.x * line2.z) / det;

    return ax::Vec2(x, y);
}


}  // namespace helper
