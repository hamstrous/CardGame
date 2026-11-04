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
    enum BorderPosition
    {
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        TOP_RIGHT,
        TOP_LEFT,
        LEFT_BORDER,
        RIGHT_BORDER,
        TOP_BORDER,
        BOTTOM_BORDER,
        NONE
    };

public:

    // Factory method
    static Widget* create();
    static Widget* createAndAddToLayer();
    bool init();
    void update(float delta) override;

    virtual void setUp();

    Widget() = default;
    ~Widget() = default;    

    virtual bool onMouseDown(ax::Event* event);
    virtual bool onMouseMove(ax::Event* event);

    virtual void drawProperties();
    virtual void redrawVisual();
    virtual void clearVisual();
    virtual void clearTransformModifyVisual();
    
    virtual void drawHoverVisual();
    virtual void clearHoverVisual();

    virtual void drawSelectedVisual();
    virtual void clearSelectedVisual();

    virtual void drawScalableBorder();
    virtual void clearScalableBorder();

    virtual void drawRotatableCircle();
    virtual void clearRotatableCircle();

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

    BorderPosition isPointOnBorder(const Vec2& point, float threshold = 10.f) const;
    bool isPointOnRotatableCircle(const Vec2& point, float threshold = 10.f) const;

    std::unordered_map<BorderPosition, Vec2> _borderPointsNormalized = {
        {BorderPosition::BOTTOM_LEFT, Vec2(0, 0)},    {BorderPosition::BOTTOM_RIGHT, Vec2(1, 0)},
        {BorderPosition::TOP_RIGHT, Vec2(1, 1)},      {BorderPosition::TOP_LEFT, Vec2(0, 1)},
        {BorderPosition::LEFT_BORDER, Vec2(0, 0.5f)}, {BorderPosition::RIGHT_BORDER, Vec2(1, 0.5f)},
        {BorderPosition::TOP_BORDER, Vec2(0.5f, 1)},  {BorderPosition::BOTTOM_BORDER, Vec2(0.5f, 0)}};

    const Vec2 _rotatableCircleNormalized = Vec2(0.5f, -0.2f);

    Vec2 getBorderPoint(BorderPosition position, float offset = 0.f) const;
    void setType(std::string type) { _type = type; }

    static int _count;
    int _id;
    std::string _type = "Widget";

    EventListenerMouse* _mouseListener;

    //Visuals
    DrawNode* _hoverVisual;
    DrawNode* _selectedVisual;
    DrawNode* _scalableBorderVisual;
    DrawNode* _rotatableCircleVisual;
    bool _hoverVisualActive = false;
    bool _selectedVisualActive = false;
    bool _scalableBorderVisualActive = false;
    bool _rotatableCircleVisualActive = false;



private:
    DrawNode* _placeholderVisual;

};
