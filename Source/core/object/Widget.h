#pragma once

#include "axmol.h"

#include <string>
#include <map>

#include "utils/helper.h"
#include "utils/Timer.hpp"

using namespace ax; 

class Widget : public Node
{

public:

    // Factory method
    static Widget* create();
    bool init();
    void update(float delta) override;

    Widget() = default;
    ~Widget() = default;

    virtual bool onMouseDown(ax::Event* event);
    virtual bool onMouseMove(ax::Event* event);

    virtual void drawProperties();
    virtual void redrawVisual();

    virtual void drawHoverVisual();
    void clearHoverVisual();

    virtual void drawScalableBorder();
    void clearScalableBorder();

    void clearVisuals()
    {
        clearHoverVisual();
        clearScalableBorder();
    }

    int getCount() const { return _count; }

    virtual std::string getId() const { return getType() + "_" + std::to_string(_id); }

    virtual std::string getType() const { return "Widget"; }

    Rect getWorldBoundingBox() const {
        // Axmol v3 has this
        auto& contentSize = getContentSize();
        Rect rect         = Rect(0, 0, contentSize.width, contentSize.height);
        return RectApplyTransform(rect, getNodeToWorldTransform());
    }

protected:
    void markTransformDirty()
    {
        this->_rotationZ_Y = this->_rotationZ_X;
        _transformUpdated = _transformDirty = _inverseDirty = true;
        _anchorPointInPoints.set(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y);
        updateRotationQuat();  // for updating rotation
        redrawVisual();
    }

    enum CornerOrBorderPosition
    {
        BottomLeft,
        BottomRight,
        TopRight,
        TopLeft,
        LeftBorder,
        RightBorder,
        TopBorder,
        BottomBorder
    };

    Vec2 getCornerOrBorderPoint(CornerOrBorderPosition position, float offset = 0.f) const
    {
        Vec2 point;
        float width   = this->getContentSize().width;
        float height  = this->getContentSize().height;
        float centerX = width / 2.f;
        float centerY = height / 2.f;
        switch (position)
        {
        case CornerOrBorderPosition::BottomLeft: 
            point = Vec2(0, 0) - Vec2(offset, offset);
            break;
        case CornerOrBorderPosition::BottomRight:  
            point = Vec2(width, 0) + Vec2(offset, -offset);
            break;
        case CornerOrBorderPosition::TopRight: 
            point = Vec2(width, height) + Vec2(offset, offset);
            break;
        case CornerOrBorderPosition::TopLeft:
            point = Vec2(0, height) + Vec2(-offset, offset);
            break;
        case CornerOrBorderPosition::LeftBorder:
            point = Vec2(0, centerY) + Vec2(-offset, 0);
            break;
        case CornerOrBorderPosition::RightBorder:
            point = Vec2(width, centerY) + Vec2(offset, 0);
            break;
        case CornerOrBorderPosition::TopBorder:
            point = Vec2(centerX, height) + Vec2(0, offset);
            break;
        case CornerOrBorderPosition::BottomBorder:
            point = Vec2(centerX, 0) + Vec2(0, -offset);
            break;
        default:
            point = Vec2(0, 0);
            AXLOGD("Widget::getCornerOrBorderPoint: Invalid position enum value");
            break;
        }
        return point + Vec2(offset, offset);
    }


    static int _count;
    int _id;

    // General properties
    bool _hide = false;
    bool _scalable = true;
    bool _rotatable = true;
        
    // Edit mode properties
    bool _hideInEditMode = false;


    // Play mode properties
    bool _allowClick = true;
    bool _allowDrag  = true;
    int _enlargeFactor = 0; // pop up a large view when hover; max = 4


    EventListenerMouse* _mouseListener;

    //Visuals
    DrawNode* _hoverVisual;
    DrawNode* _scalableBorderVisual;
    bool _hoverVisualActive = false;
    bool _scalableBorderVisualActive = false;



private:
    DrawNode* _placeholderVisual;

};
