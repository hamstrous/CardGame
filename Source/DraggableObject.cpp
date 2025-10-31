#include "DraggableObject.h"
using namespace ax;

bool DraggableObject::containsPoint(const ax::Vec2& worldPoint) const
{
    Vec2 localPoint = this->convertToNodeSpace(worldPoint);
    Rect bbox       = Rect(-_objectSize.x / 2, -_objectSize.y / 2, _objectSize.x, _objectSize.y);
    return bbox.containsPoint(localPoint);
}

void DraggableObject::enableDragging(bool enable)
{
    _isDraggable = enable;
}

void DraggableObject::setHighlight(bool highlight)
{
    if (highlight && !_highlightNode)
    {
        _highlightNode = ax::DrawNode::create();
        Vec2 rect[4];
        float w = _objectSize.x;
        float h = _objectSize.y;
        rect[0] = Vec2(-w / 2, -h / 2);
        rect[1] = Vec2(w / 2, -h / 2);
        rect[2] = Vec2(w / 2, h / 2);
        rect[3] = Vec2(-w / 2, h / 2);
        _highlightNode->drawPoly(rect, 4, true, Color4F::YELLOW);
        this->addChild(_highlightNode, 100);
        this->setScale(_originalScale * 1.05f);
    }
    else if (!highlight && _highlightNode)
    {
        this->removeChild(_highlightNode);
        _highlightNode = nullptr;
        this->setScale(_originalScale);
    }
}
