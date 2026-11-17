#include "ScreenHelper.h"
#include "helper.h"

ax::Vec2 ScreenHelper::convertScreenPositionToWorld(ax::Node* node, ax::Vec2 screenPosition)
{
    AXASSERT(node != nullptr, "Node is null");
    auto runningScene = node->getScene();
    if (dynamic_cast<ax::Scene*>(node))
        runningScene = dynamic_cast<ax::Scene*>(node);
    AXASSERT(runningScene != nullptr, "Node does not belong to any scene");
    auto screenSize             = ax::Director::getInstance()->getVisibleSize();
    int zoomFactor              = (runningScene->getDefaultCamera())->getZoom();
    auto camPosition            = (runningScene->getDefaultCamera())->getPosition();
    auto camSize                = screenSize * zoomFactor;
    ax::Vec2 camStartBottomLeft = camPosition - (camSize / 2);

    auto screenPositionRatio = screenPosition / screenSize;

    ax::Vec2 worldPosition = camStartBottomLeft + screenPositionRatio * camSize;

    return worldPosition;
}

ax::Vec2 ScreenHelper::convertWorldPositionToScreen(ax::Node* node, ax::Vec2 worldPosition)
{
    AXASSERT(node != nullptr, "Node is null");
    auto runningScene = node->getScene();
    if (dynamic_cast<ax::Scene*>(node))
        runningScene = dynamic_cast<ax::Scene*>(node);
    AXASSERT(runningScene != nullptr, "Node does not belong to any scene");
    auto screenSize             = ax::Director::getInstance()->getVisibleSize();
    int zoomFactor              = (runningScene->getDefaultCamera())->getZoom();
    auto camPosition            = (runningScene->getDefaultCamera())->getPosition();
    auto camSize                = screenSize * zoomFactor;
    ax::Vec2 camStartBottomLeft = camPosition - (camSize / 2);

    auto worldPositionRatio = (worldPosition - camStartBottomLeft) / camSize;

    ax::Vec2 screenPosition = worldPositionRatio * screenSize;

    return screenPosition;
}

void ScreenHelper::getWorldPositionForEnlargePreview(ax::Node* node, ax::Sprite* enlargeSprite, float enlargeRatio)
{
    auto nodeScreenPosition = convertWorldPositionToScreen(node, helper::getNodePositionInWorldSpace(node));
    auto nodeSize           = node->getContentSize();
    auto enlargeSize        = node->getContentSize() * enlargeRatio; // temp
    auto screenSize         = ax::Director::getInstance()->getVisibleSize();
    auto ret                = ax::Vec2(0, 0);

    bool moveRight = nodeScreenPosition.x < (screenSize.x / 2);
    bool moveUp    = nodeScreenPosition.y < (screenSize.y / 2);
    bool stickHorizontally = nodeSize.width <= nodeSize.height;

    float gap = 20.f;

    float horizontalPosition, horizontalOffset, horizontalPositive;
    float verticalPosition, verticalOffset, verticalPositive;

    horizontalPositive = moveRight ? 1 : -1;
    horizontalOffset   = nodeSize.width / 2 + enlargeSize.width / 2;
    horizontalPosition = (horizontalOffset + gap) * horizontalPositive;

    verticalPositive = moveUp ? 1 : -1;
    verticalOffset   = nodeSize.height / 2 + enlargeSize.height / 2;
    verticalPosition = (verticalOffset + gap) * verticalPositive;

    // Only use vertical position if needed
    bool needVertical = (nodeScreenPosition.y - enlargeSize.height / 2 < 0) ||
                        (nodeScreenPosition.y + enlargeSize.height / 2 > screenSize.height);

    // If use both but still out of screen then just render it in the middle
    auto spriteUseBothScreenPosition = nodeScreenPosition + ax::Vec2(horizontalPosition, verticalPosition);

    bool isOutOfScreen = spriteUseBothScreenPosition.x - enlargeSize.width / 2 < 0 ||
                         spriteUseBothScreenPosition.x + enlargeSize.width / 2 > screenSize.width ||
                         spriteUseBothScreenPosition.y - enlargeSize.height / 2 < 0 ||
                         spriteUseBothScreenPosition.y + enlargeSize.height / 2 > screenSize.height;

    ret = isOutOfScreen ? screenSize / 2 : nodeScreenPosition + ax::Vec2(horizontalPosition, needVertical ? verticalPosition : 0);

    enlargeSprite->setPosition(convertScreenPositionToWorld(node, ret));
    enlargeSprite->setContentSize(enlargeSize);
}
