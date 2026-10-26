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

    bool isWidgetIntersectingRubberBand(Widget* widget);
    bool isWidgetSelected(Widget* widget) {
        return _selectedWidgets.find(widget) != _selectedWidgets.end();
    }

    void addSelectedWidget(Widget* widget) {
        _selectedWidgets.insert(widget);
        widget->drawHoverVisual();
    }

    void removeSelectedWidget(Widget* widget) {
        _selectedWidgets.erase(widget);
        widget->clearHoverVisual();
    }

    void clearSelectedWidgets()
    {
        for (auto widget : _selectedWidgets)
        {
            widget->clearHoverVisual();
        }
        _selectedWidgets.clear();
    }

    set<Widget*>& getSelectedWidgets() { return _selectedWidgets; }
    Vec2 getMousePosition() const { return _mousePreviousPosition; }
    Widget* getHoveringWidget() const { return _hoveringWidget; }

    bool isDragging() const { return _dragging; }
    bool isRubberBanding() const { return _rubberBanding; }

    void justClickedInsideWidget(Widget* widget);
    void hoveringInsideWidget(Widget* widget);

private:
    static InputManager* _instance;

    EventListenerMouse* _mouseBeforeListener;
    EventListenerMouse* _mouseAfterListener;
    EventListenerKeyboard* _keyboardListener;

    DrawNode* _drawNode;  // For drawing rubber band selection
    set<Widget*> _selectedWidgets;  // Store selected widgets
    Widget* _hoveringWidget = nullptr;

    set<EventKeyboard::KeyCode> _pressingKeys;

    Vec2 _startPoint;  // Starting point of the selection rectangle
    Vec2 _mousePreviousPosition;  // Previous mouse position for dragging

    bool _justClickedInsideWidget = false;  
    bool _dragging = false; 
    bool _rubberBanding = false;
};
