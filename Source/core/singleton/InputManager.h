#pragma once

#include "axmol.h"

#include "core/object/Widget.h"
#include "core/const/GameConstants.h"

#include "core/event/EventMode.h"
#include "core/event/EventListenerMode.h"

#include "ImGui/ImGuiPresenter.h"

USING_NS_AX_EXT;
using namespace ax;
using namespace std;
using namespace constant;

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

    bool onModeChanged(EventMode* mode);

    void startRubberBandSelection(const Vec2& startPoint);
    void endRubberBandSelection(const Vec2& endPoint);
    void updateRubberBandSelection(const Vec2& mousePosition);

    void startDragging();
    void endDragging();
    void updateDragging(const Vec2& mousePosition);

    void startScaling(const Vec2& normalizedBorderPoint);
    void endScaling();
    void updateScaling(const Vec2& mousePosition);

    void startRotating();
    void endRotating();
    void updateRotating(const Vec2& mousePosition);

    bool isWidgetIntersectingRubberBand(Widget* widget);
    bool isWidgetSelected(Widget* widget) {
        return _selectedWidgets.find(widget) != _selectedWidgets.end();
    }

    void addSelectedWidget(Widget* widget) {
        _selectedWidgets.insert(widget);
        widget->drawSelectedVisual();
        CheckAloneWidget();
    }

    void removeSelectedWidget(Widget* widget) {
        // This is the only place where we clear the visuals of a widget when it is deselected
        widget->clearVisual();
        _selectedWidgets.erase(widget);
        CheckAloneWidget();
    }

    void CheckAloneWidget();

    void clearSelectedWidgets()
    {
        for (auto widget : _selectedWidgets)
        {
            widget->clearVisual();
        }
        _selectedWidgets.clear();
    }

    set<Widget*>& getSelectedWidgets() { return _selectedWidgets; }
    Vec2 getMousePosition() const { return _mousePreviousPosition; }
    Widget* getHoveringWidget() const { return _hoveringWidget; }

    bool isDragging() const { return _dragging; }
    bool isRubberBanding() const { return _rubberBanding; }
    bool isScaling() const { return _scaling; }
    bool isRotating() const { return _rotating; }

    void justClickedInsideWidget(Widget* widget);
    bool justClickedOnWidgetBorder(Widget* widget, Vec2 normalizedBorderPoint);
    bool justClickedOnWidgetCircle(Widget* widget);
    void hoveringInsideWidget(Widget* widget);

    //void changeMode(const Mode& mode);

private:
    static InputManager* _instance;

    EventListenerMouse* _mouseBeforeListener;
    EventListenerMouse* _mouseAfterListener;
    EventListenerKeyboard* _keyboardListener;
    EventListenerMode* _modeListener;

    DrawNode* _drawNode;  // For drawing rubber band selection
    set<Widget*> _selectedWidgets;  // Store selected widgets
    Widget* _hoveringWidget = nullptr;
    Widget* _selectedAloneWidget  = nullptr;

    set<EventKeyboard::KeyCode> _pressingKeys;

    Vec2 _startPoint;  // Starting point of the selection rectangle
    Vec2 _mousePreviousPosition;  // Previous mouse position for dragging/scaling/rotating

    bool _justClickedInsideWidget = false;
    bool _dragging                = false;
    bool _scaling                 = false;
    bool _rotating                = false;
    bool _rubberBanding           = false;

    // Transform related
    Vec2 _normalizedBorderPoint;  // Normalized point on the widget border where scaling started
    Vec2 _previousAnchorPoint;   

    bool _canDrag         = true;
    bool _canScale        = true;
    bool _canRotate       = true;
    bool _canRubberband   = true;
    bool _canEnlarge      = false;
    bool _toggleSelection   = false; //Click on widget to toggle select

};
