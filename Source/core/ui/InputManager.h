#pragma once

#include "axmol.h"

#include "core/object/Widget.h"

#include "ImGui/ImGuiPresenter.h"

USING_NS_AX_EXT;
using namespace ax;
using namespace std;

class InputManager : public ax::Node
{
public:
    InputManager();

    static InputManager* getInstance();
    static void destroyInstance();

    bool onMouseDownBefore(Event* event);
    bool onMouseDownAfter(Event* event);
    bool onMouseMoveBefore(Event* event);
    bool onMouseMoveAfter(Event* event);
    bool onMouseUp(Event* event);

    bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    void startRubberBandSelection(const Vec2& startPoint);
    void endRubberBandSelection(const Vec2& endPoint);
    void updateRubberBandSelection(const Vec2& currentPoint);

    void startDragging();
    void endDragging();
    void updateDragging(const Vec2& currentPoint);

    void startScaling(const Vec2& normalizedBorderPoint);
    void endScaling();
    void updateScaling(const Vec2& currentPoint);

    bool isWidgetIntersectingRubberBand(Widget* widget);
    bool isWidgetSelected(Widget* widget) {
        return _selectedWidgets.find(widget) != _selectedWidgets.end();
    }

    void addSelectedWidget(Widget* widget) {
        _selectedWidgets.insert(widget);
        widget->drawHoverVisual();
        CheckAloneScalableWidget();
    }

    void removeSelectedWidget(Widget* widget) {
        widget->clearScalableBorder();
        _selectedWidgets.erase(widget);
        widget->clearHoverVisual();
        CheckAloneScalableWidget();
    }

    void CheckAloneScalableWidget()
    {
        if (_selectedWidgets.size() == 1)
        {
            (*_selectedWidgets.begin())->drawScalableBorder();
        }
        else
        {
            for (auto widget : _selectedWidgets)
            {
                widget->clearScalableBorder();
            }
        }
    }

    void clearSelectedWidgets()
    {
        for (auto widget : _selectedWidgets)
        {
            widget->clearHoverVisual();
            widget->clearScalableBorder();
        }
        _selectedWidgets.clear();
    }

    set<Widget*>& getSelectedWidgets() { return _selectedWidgets; }
    Vec2 getMousePosition() const { return _mousePreviousPosition; }
    Widget* getHoveringWidget() const { return _hoveringWidget; }

    bool isDragging() const { return _dragging; }
    bool isRubberBanding() const { return _rubberBanding; }
    bool isScaling() const { return _scaling; }

    void justClickedInsideWidget(Widget* widget);
    bool justClickedOnWidgetBorder(Widget* widget, Vec2 normalizedBorderPoint);
    void hoveringInsideWidget(Widget* widget);

private:
    static InputManager* _instance;

    EventListenerMouse* _mouseBeforeListener;
    EventListenerMouse* _mouseAfterListener;
    EventListenerKeyboard* _keyboardListener;

    DrawNode* _drawNode;  // For drawing rubber band selection
    set<Widget*> _selectedWidgets;  // Store selected widgets
    Widget* _hoveringWidget = nullptr;
    Widget* _scalingWidget  = nullptr;

    set<EventKeyboard::KeyCode> _pressingKeys;

    Vec2 _startPoint;  // Starting point of the selection rectangle
    Vec2 _mousePreviousPosition;  // Previous mouse position for dragging/scaling/rotating

    bool _justClickedInsideWidget = false;
    bool _dragging                = false;
    bool _scaling                 = false;
    bool _rubberBanding           = false;

    // Scaling related
    Vec2 _normalizedBorderPoint;  // Normalized point on the widget border where scaling started
    Vec2 _previousAnchorPoint;   
    bool _scalingX = false;
    bool _scalingY = false;

};
