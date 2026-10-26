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
    bool onMouseMove(Event* event);
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

    void addSelectedWidget(Widget* widget) {
        _selectedWidgets.insert(widget);
    }

    void removeSelectedWidget(Widget* widget) {
        _selectedWidgets.erase(widget);
    }

    set<Widget*>& getSelectedWidgets() { return _selectedWidgets; }

    bool isDragging() const { return _dragging; }
    bool isRubberBanding() const { return _rubberBanding; }

    void justClickedInsideWidget(Widget* widget) {
        _justClickedInsideWidget = true;

        // If click on the same set of selected widgets
        // Then we can drag all
        // Else this is the new selected widget

        bool isShiftPressed = _pressingKeys.find(EventKeyboard::KeyCode::KEY_SHIFT) != _pressingKeys.end();

        if (_selectedWidgets.find(widget) == _selectedWidgets.end() && !isShiftPressed)
        {
            _selectedWidgets.clear();
            _selectedWidgets.insert(widget);
        }
        else if (isShiftPressed)
        {
            _selectedWidgets.insert(widget);
        }
        startDragging();
    }

private:
    static InputManager* _instance;

    EventListenerMouse* _mouseBeforeListener;
    EventListenerMouse* _mouseAfterListener;
    EventListenerKeyboard* _keyboardListener;

    DrawNode* _drawNode;  // For drawing rubber band selection
    set<Widget*> _selectedWidgets;  // Store selected widgets

    set<EventKeyboard::KeyCode> _pressingKeys;

    Vec2 _startPoint;  // Starting point of the selection rectangle
    Vec2 _mousePreviousPosition;  // Previous mouse position for dragging

    bool _justClickedInsideWidget = false;  
    bool _dragging = false; 
    bool _rubberBanding = false;
};
