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
