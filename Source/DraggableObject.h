#pragma once

#include "axmol.h"

class DraggableObject : public ax::Node
{
public:
    virtual bool containsPoint(const ax::Vec2& worldPoint) const;

    bool isDragging() const { return _isDragging; }
    void enableDragging(bool enable);

    void setDragOffset(const ax::Vec2& offset) { _dragOffset = offset; }
    ax::Vec2 getDragOffset() const { return _dragOffset; }

    void setOriginalPosition(const ax::Vec2& pos) { _originalPosition = pos; }
    ax::Vec2 getOriginalPosition() const { return _originalPosition; }

    virtual void startDragging() { _isDragging = true; }
    void stopDragging() { _isDragging = false; }

    virtual void setHighlight(bool highlight);

protected:
    ax::Vec2 _objectSize;

    bool _isDraggable = true;
    bool _isDragging  = false;

    ax::Vec2 _dragOffset;
    ax::Vec2 _mouseDownPosition;
    ax::Vec2 _originalPosition;

    float _originalScale         = 1.0f;
    ax::DrawNode* _highlightNode = nullptr;
};
