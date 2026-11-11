#pragma once

#include "axmol.h"

#include <string>
#include <map>

#include "utils/helper.h"
#include "utils/Timer.hpp"

#include "core/ui/data/InspectorData.h"

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
    void addToLayer();
    bool init();
    virtual Widget* clone() const;

    void update(float delta) override;

    virtual void setUpInteractEffect();
    virtual void setUpMainVisual();

    Widget() = default;
    ~Widget() = default;    

    virtual bool onMouseDown(ax::Event* event);
    virtual bool onMouseMove(ax::Event* event);

    virtual void drawProperties();

    virtual InspectorData createInspectorData();
    InspectorData& getInspectorData() { return _inspectorData; }

    virtual void redrawEffect();
    virtual void clearEffect();
    virtual void clearTransformModifyEffect();
    
    virtual void drawHoverEffect();
    virtual void clearHoverEffect();

    virtual void drawSelectedEffect();
    virtual void clearSelectedEffect();

    virtual void drawScalableBorder();
    virtual void clearScalableBorder();

    virtual void drawRotatableCircle();
    virtual void clearRotatableCircle();

    void drawEmphasisEffect();
    void clearEmphasisEffect();

    void drawTransparentEffect();
    void clearTransparentEffect();

    int getCount() const { return _count; }

    virtual std::string getId() const { return getType() + "_" + std::to_string(_id); }

    virtual std::string getType() const { return "Widget"; }

    Rect getWorldBoundingBox() const {
        // Axmol v3 has this
        auto& contentSize = getContentSize();
        Rect rect         = Rect(0, 0, contentSize.width, contentSize.height);
        return RectApplyTransform(rect, getNodeToWorldTransform());
    }

    void markTransformDirty()
    {
        this->_rotationZ_Y = this->_rotationZ_X;
        _transformUpdated = _transformDirty = _inverseDirty = true;
        _anchorPointInPoints.set(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y);
        updateRotationQuat();  // for updating rotation
        redrawEffect();
    }

    set<int>& getAllowedRotation() { return _allowedRotation; }
    void addAllowedRotation(int rotation)
    {
        if (rotation == 360 || rotation == 0)
        {
            _allowedRotation.insert(0);
            _allowedRotation.insert(-360);
            _allowedRotation.insert(360);
            return;
        }
        _allowedRotation.insert(rotation % 360);
        _allowedRotation.insert(-(360 - (rotation % 360)));
    }
    void eraseAllowedRotation(int rotation)
    {
        if (rotation == 360 || rotation == 0)
        {
            _allowedRotation.erase(0);
            _allowedRotation.erase(-360);
            _allowedRotation.erase(360);
            return;
        }
        _allowedRotation.erase(rotation % 360);
        _allowedRotation.erase(-(360 - (rotation % 360)));
    }
    void rotateToNeareastAllowedRotation();

protected:
    
    BorderPosition isPointOnBorder(const Vec2& point, float threshold = 10.f) const;
    bool isPointOnRotatableCircle(const Vec2& point, float threshold = 10.f) const;

    std::unordered_map<BorderPosition, Vec2> _borderPointsNormalized = {
        {BorderPosition::BOTTOM_LEFT, Vec2(0, 0)},    {BorderPosition::BOTTOM_RIGHT, Vec2(1, 0)},
        {BorderPosition::TOP_RIGHT, Vec2(1, 1)},      {BorderPosition::TOP_LEFT, Vec2(0, 1)},
        {BorderPosition::LEFT_BORDER, Vec2(0, 0.5f)}, {BorderPosition::RIGHT_BORDER, Vec2(1, 0.5f)},
        {BorderPosition::TOP_BORDER, Vec2(0.5f, 1)},  {BorderPosition::BOTTOM_BORDER, Vec2(0.5f, 0)}};

    const Vec2 _rotatableCirclePosition = Vec2(0.5f, -25.f); // x is normalize, y is absolute

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

    //Gui
    InspectorData _inspectorData;

    //Action
    float _previousRotation;

    set<int> _allowedRotation;


private:
    DrawNode* _placeholderVisual;

};
