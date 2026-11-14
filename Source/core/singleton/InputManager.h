#pragma once

#include "axmol.h"

class EventMode;
class EventListenerMode;
class Widget;
class LayerManager;
class Inspector;

class InputManager : public ax::Node
{
public:
    InputManager();

    static InputManager* getInstance();
    static void destroyInstance();

    bool onMouseDownBefore(ax::Event* event);
    bool onMouseDownAfter(ax::Event* event);
    bool onMouseMoveBefore(ax::Event* event);
    bool onMouseMoveAfter(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    bool onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    bool onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

    bool onModeChanged(EventMode* mode);

    void startRubberBandSelection(const ax::Vec2& startPoint);
    void endRubberBandSelection(const ax::Vec2& endPoint);
    void updateRubberBandSelection(const ax::Vec2& mousePosition);

    void startDragging();
    void endDragging();
    void updateDragging(const ax::Vec2& mousePosition);

    void startScaling(const ax::Vec2& normalizedBorderPoint);
    void endScaling();
    void updateScaling(const ax::Vec2& mousePosition);

    void startRotating();
    void endRotating();
    void updateRotating(const ax::Vec2& mousePosition);

    bool isWidgetIntersectingRubberBand(Widget* widget);
    bool isWidgetSelected(Widget* widget);

    void addSelectedWidget(Widget* widget);
    void removeSelectedWidget(Widget* widget);

    void checkAloneWidget();

    void clearSelectedWidgets();
    
    std::set<Widget*>& getSelectedWidgets() { return _selectedWidgets; }
    std::vector<Widget*>& getSortedSelectedWidgets();

    ax::Vec2 getMousePosition() const { return _mousePreviousPosition; }
    Widget* getHoveringWidget() const { return _hoveringWidget; }

    void duplicateSelectedWidgets();
    void deleteSelectedWidgets();

    bool isDragging() const { return _dragging; }
    bool isRubberBanding() const { return _rubberBanding; }
    bool isScaling() const { return _scaling; }
    bool isRotating() const { return _rotating; }

    void justClickedInsideWidget(Widget* widget);
    bool justClickedOnWidgetBorder(Widget* widget, ax::Vec2 normalizedBorderPoint);
    bool justClickedOnWidgetCircle(Widget* widget);
    void hoveringInsideWidget(Widget* widget);

    //void changeMode(const Mode& mode);

private:
    static InputManager* _instance;

    LayerManager* _layerManager = nullptr;

    ax::EventListenerMouse* _mouseBeforeListener;
    ax::EventListenerMouse* _mouseAfterListener;
    ax::EventListenerKeyboard* _keyboardListener;
    EventListenerMode* _modeListener;

    ax::DrawNode* _drawNode;  // For drawing rubber band selection
    std::set<Widget*> _selectedWidgets;  // Store selected widgets
    Widget* _hoveringWidget = nullptr;
    Widget* _selectedAloneWidget  = nullptr;

    std::set<ax::EventKeyboard::KeyCode> _pressingKeys;

    ax::Vec2 _startPoint;  // Starting point of the selection rectangle
    ax::Vec2 _mousePreviousPosition;  // Previous mouse position for dragging/scaling/rotating

    bool _justClickedInsideWidget = false;
    bool _dragging                = false;
    bool _scaling                 = false;
    bool _rotating                = false;
    bool _rubberBanding           = false;

    // Transform related
    ax::Vec2 _normalizedBorderPoint;  // Normalized point on the widget border where scaling started
    ax::Vec2 _previousAnchorPoint;   

    bool _canDrag                = true;
    bool _canScale               = true;
    bool _canRotate              = true;
    bool _canRubberband          = true;
    bool _canEnlarge             = false;
    bool _toggleSelection        = false;  // Click on widget to toggle select
    bool _dragMoveWidgetsToFront = true;
    bool _dragEmphasisEffect     = true;

};
