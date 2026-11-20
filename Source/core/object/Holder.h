#pragma once

#include "axmol.h"

#include "core/object/Widget.h"

class Holder : public Widget
{

public:

    // Factory method
    static Holder* create();
    void addToLayer();
    bool init();
    Holder* clone() const override;

    void drawGui() override;

    void update(float delta) override;

    // Override
    void setContentSize(const ax::Size &size) override;
    void setUpMainVisual() override;

    Holder() = default;
    ~Holder() = default;    

    //InspectorData createInspectorData() override;

    std::string getType() const override { return "Holder"; }


    //Widget is checking to find the holder is touching the most
    //Even if this function return true is it not certain that it is hovering on this holder
    bool isWidgetHoveringOn(Widget* widget, ax::Vec2& mtv);

    //Widget is hovering on this holder 
    void otherHoveringOnAbove(Widget* widget) override;
    void otherStopHoveringOnAbove(Widget* widget) override;

    void onWidgetEnter(Widget* widget);
    void onWidgetLeave(Widget* widget);

    void moveWidgetBackToPosition();
    void moveOtherWidgetsAside(int snapIndex);

    void updateSnapPositionList();

    std::vector<ax::Vec2> getAvailableSnapPosition() { return _snapPositionList; }
    std::vector<ax::Vec2> getAvailableWorldSnapPosition();
    ax::Vec2 getWorldSnapPositionAtIndex(int index);
    int getIndexOfClosestSnapPoint(Widget* widget);
    ax::Vec2 getClosestWorldSnapPointWithWidgetOffset(Widget* widget);
    ax::Vec2 getSnapPositionInNodeParentSpaceAtIndex(Widget* widget, int index);

protected:
    int _widgetHolderActionTag; 
    static int _count;
    int _id;

    float _roundedCorner = 0.5;
    bool _alignChilren   = true;
    ax::Vec2 _spreadDirection = ax::Vec2::UNIT_X;
    int _dropLimit       = -1;
    bool _showLimit      = false;
    bool _dropShadow     = true;
    bool _preventPile    = false;
    std::vector<std::string> _dropTarget;
    ax::Vec2 _targetDropPosition;
    bool _previouslyHoveredOn = false;
    ax::Sprite* _shadow       = nullptr;
    ax::Vec2 _stackOffset, _dropOffset;

    std::vector<ax::Vec2> _snapPositionList; // In node space
    std::vector<Widget*> _childrenWithOrder; //Index of children

    int _currentHoveringIndex = -1; //Where is the widget hover on


};
