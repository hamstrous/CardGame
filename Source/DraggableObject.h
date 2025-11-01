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

    virtual bool isDraggable() { return _isDraggable; }

    virtual ax::Rect getBoundingBox() const
    {
        ax::Vec2 worldPos = this->getParent()->convertToWorldSpace(this->getPosition());
        return ax::Rect(worldPos.x - _objectSize.x / 2, worldPos.y - _objectSize.y / 2, _objectSize.x, _objectSize.y);
    }

    // Static helper function to sort objects by position
    template <typename T>
    static void sortObjectsByPosition(std::vector<T>& objects)
    {
        std::sort(objects.begin(), objects.end(), [](T a, T b) {
            ax::Vec2 posA = a->getPosition();
            ax::Vec2 posB = b->getPosition();
            if (posA.x != posB.x)
            {
                return posA.x < posB.x;
            }
            return posA.y < posB.y;
        });
    }

    void rotate(float angle)
    {
        if (this->getNumberOfRunningActions() == 0)
        {
            auto rotateBy = ax::RotateBy::create(0.2f, angle);
            this->runAction(rotateBy);
        }
    }
    void rotateSmooth(float angle)
    {
        auto rotateBy = ax::RotateBy::create(0.08f, angle);
        this->runAction(rotateBy);
    }

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
