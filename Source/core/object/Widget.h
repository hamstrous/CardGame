#pragma once

#include "axmol.h"

#include "core/ui/data/InspectorData.h"

class Widget : public ax::Node
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
    virtual void drawModifyTransformEffect();
    virtual void clearModifyTransformEffect();
    
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

    ax::Rect getWorldBoundingBox() const {
        // Axmol v3 has this
        auto& contentSize = getContentSize();
        ax::Rect rect         = ax::Rect(0, 0, contentSize.width, contentSize.height);
        return ax::RectApplyTransform(rect, getNodeToWorldTransform());
    }

    void markTransformDirty();
    

    std::set<int>& getAllowedRotation() { return _allowedRotation; }
    void addAllowedRotation(int rotation);
    void eraseAllowedRotation(int rotation);
    void rotateToNeareastAllowedRotation();

protected:
    
    BorderPosition isPointOnBorder(const ax::Vec2& point, float threshold = 10.f) const;
    bool isPointOnRotatableCircle(const ax::Vec2& point, float threshold = 10.f) const;

    std::unordered_map<BorderPosition, ax::Vec2> _borderPointsNormalized = {
        {BorderPosition::BOTTOM_LEFT, ax::Vec2(0, 0)},    {BorderPosition::BOTTOM_RIGHT, ax::Vec2(1, 0)},
        {BorderPosition::TOP_RIGHT, ax::Vec2(1, 1)},      {BorderPosition::TOP_LEFT, ax::Vec2(0, 1)},
        {BorderPosition::LEFT_BORDER, ax::Vec2(0, 0.5f)}, {BorderPosition::RIGHT_BORDER, ax::Vec2(1, 0.5f)},
        {BorderPosition::TOP_BORDER, ax::Vec2(0.5f, 1)},  {BorderPosition::BOTTOM_BORDER, ax::Vec2(0.5f, 0)}};

    const ax::Vec2 _rotatableCirclePosition = ax::Vec2(0.5f, -25.f); // x is normalize, y is absolute

    ax::Vec2 getBorderPoint(BorderPosition position, float offset = 0.f) const;
    void setType(std::string type) { _type = type; }

    static int _count;
    int _id;
    std::string _type = "Widget";

    ax::EventListenerMouse* _mouseListener;

    //Visuals
    ax::DrawNode* _hoverVisual;
    ax::DrawNode* _selectedVisual;
    ax::DrawNode* _scalableBorderVisual;
    ax::DrawNode* _rotatableCircleVisual;
    bool _hoverVisualActive = false;
    bool _selectedVisualActive = false;
    bool _scalableBorderVisualActive = false;
    bool _rotatableCircleVisualActive = false;

    //Gui
    InspectorData _inspectorData;

    //Action
    float _previousRotation;

    std::set<int> _allowedRotation;


private:
    ax::DrawNode* _placeholderVisual;

};
