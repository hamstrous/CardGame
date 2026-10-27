#include "InputManager.h"

using namespace helper;

InputManager* InputManager::_instance = nullptr;

InputManager::InputManager()
{
    _drawNode = ax::DrawNode::create();
    _drawNode->setVisible(false);
    this->addChild(_drawNode);

    _startPoint = Vec2::ZERO;

    _mouseAfterListener = ax::EventListenerMouse::create();
    _mouseAfterListener->onMouseDown = AX_CALLBACK_1(InputManager::onMouseDownAfter, this);
    _mouseAfterListener->onMouseMove = AX_CALLBACK_1(InputManager::onMouseMoveAfter, this);
    _mouseAfterListener->onMouseUp = AX_CALLBACK_1(InputManager::onMouseUp, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_mouseAfterListener, 1);  // get called after scene graph


    _mouseBeforeListener = ax::EventListenerMouse::create();
    _mouseBeforeListener->onMouseDown = AX_CALLBACK_1(InputManager::onMouseDownBefore, this);
    _mouseBeforeListener->onMouseMove  = AX_CALLBACK_1(InputManager::onMouseMoveBefore, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_mouseBeforeListener, -1);  // get called before scene graph

    _keyboardListener = ax::EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = AX_CALLBACK_2(InputManager::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(InputManager::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 1);

}

InputManager* InputManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new InputManager();
    }
    return _instance;
}

void InputManager::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }   
}

bool InputManager::onMouseDownBefore(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    auto mousePos     = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());

    return false;
}

bool InputManager::onMouseDownAfter(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    auto mousePos   = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    _justClickedInsideWidget = false;

    if (_dragging)
    {
        return false;
    }

    startRubberBandSelection(mousePos);

    return false;
}

bool InputManager::onMouseMoveBefore(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    auto mousePos   = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());

    if (isDragging())
    {
        updateDragging(mousePos);
    }

    if (isRubberBanding())
    {
        updateRubberBandSelection(mousePos);
    }

    if (isScaling())
    {
        updateScaling(mousePos);
    }

    _mousePreviousPosition = mousePos;

    return false;
}

bool InputManager::onMouseMoveAfter(Event* event) {
    // Not swallowed, mean hovering over nothing
    if (_hoveringWidget != nullptr)
    {
        _hoveringWidget->clearHoverVisual();
        _hoveringWidget = nullptr;
    }
    return false;
}

bool InputManager::onMouseUp(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    auto mousePos   = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
    endRubberBandSelection(mousePos);
    endDragging();
    endScaling();

    return false;
}

bool InputManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _pressingKeys.insert(keyCode);
    return false;
}

bool InputManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    _pressingKeys.erase(keyCode);
    return false;
}

void InputManager::startRubberBandSelection(const Vec2& startPoint) {
    _rubberBanding = true;
    _startPoint    = startPoint;
    _selectedWidgets.clear();   
    _drawNode->clear();
    _drawNode->setVisible(true);
    _drawNode->drawRect(_startPoint, _startPoint, Color4F::BLUE);
}

void InputManager::endRubberBandSelection(const Vec2& endPoint) {
    _rubberBanding = false;
    _drawNode->clear();
    _drawNode->setVisible(false);
}

void InputManager::updateRubberBandSelection(const Vec2& currentPoint) {
    _drawNode->clear();
    _drawNode->drawRect(_startPoint, currentPoint, Color4F::BLUE);
}

void InputManager::startDragging() {
    _dragging = true;
}

void InputManager::endDragging() {
    _dragging = false;
}

void InputManager::updateDragging(const Vec2& currentPoint) {
    // Move every selected widget by the delta of the mouse movement
    Vec2 delta = currentPoint - _mousePreviousPosition;
    for (auto widget : _selectedWidgets)
    {
        widget->setPosition(widget->getPosition() + delta);
    }
}

void InputManager::startScaling(const Vec2& normalizedBorderPoint) {
    AXASSERT(_scalingWidget != nullptr, "Scaling widget is null");
    _scaling = true;
    _normalizedBorderPoint = normalizedBorderPoint;
    _previousAnchorPoint   = this->getAnchorPoint();
    //_scalingWidget->setAnchorPoint(Vec2(1, 1) - _normalizedBorderPoint); // Set anchor point to the opposite of the border point

    setNodeAnchorPointPreservePosition(_scalingWidget, Vec2(1, 1) - _normalizedBorderPoint);

    // Because setting anchor point will change the position of the widget (visually), we need to adjust
    //_scalingWidget->setPosition(_scalingWidget->getPosition() - (_normalizedBorderPoint - _previousAnchorPoint) * _scalingWidget->getContentSize());

    if (_normalizedBorderPoint.x != 0.5f)
    {
        _scalingX = true;
    }
    if (_normalizedBorderPoint.y != 0.5f)
    {
        _scalingY = true;
    }

}

void InputManager::endScaling() {
    if (!_scaling)
        return;
    _scaling = false;
    setNodeAnchorPointPreservePosition(_scalingWidget, _previousAnchorPoint);
    _scalingWidget = nullptr;
}

void InputManager::updateScaling(const Vec2& currentPoint) {
    float deltaX = currentPoint.x - _mousePreviousPosition.x;
    float deltaY = currentPoint.y - _mousePreviousPosition.y;

    //test
    _scalingWidget->setContentSize(_scalingWidget->getContentSize() + Size(deltaX * (_scalingX ? 1 : 0), deltaY * (_scalingY ? 1 : 0)));
}

bool InputManager::isWidgetIntersectingRubberBand(Widget* widget) {
    if (!isRubberBanding())
        return false;

    auto widgetBox = widget->getWorldBoundingBox();
    auto rubberBandBox = getRectFromPoints(_startPoint, _mousePreviousPosition);

    bool isIntersecting = widgetBox.intersectsRect(rubberBandBox);

    if (isIntersecting)
    {
        addSelectedWidget(widget);
    }
    else
    {
        removeSelectedWidget(widget);
    }

    return isIntersecting;
}

void InputManager::justClickedInsideWidget(Widget* widget)
{
    _justClickedInsideWidget = true;

    // If click on the same set of selected widgets
    // Then we can drag all
    // Else this is the new selected widget

    bool isShiftPressed = _pressingKeys.find(EventKeyboard::KeyCode::KEY_SHIFT) != _pressingKeys.end();

    if (_selectedWidgets.find(widget) == _selectedWidgets.end() && !isShiftPressed)
    {
        clearSelectedWidgets();
        addSelectedWidget(widget);
    }
    else if (isShiftPressed)
    {
        addSelectedWidget(widget);
    }
    startDragging();
}

bool InputManager::justClickedOnWidgetBorder(Widget* widget, Vec2 normalizedBorderPoint) {
    // Widget must be selected to scale it
    if (_selectedWidgets.find(widget) == _selectedWidgets.end())
        return false;
    // Currently we only support scaling one widget at a time
    if (_selectedWidgets.size() != 1)
        return false;

    _scalingWidget = widget;
    startScaling(normalizedBorderPoint);
    return true;
}

void InputManager::hoveringInsideWidget(Widget* widget)
{
    if (_hoveringWidget == widget)
        return;
    if (_hoveringWidget != nullptr)
    {
        _hoveringWidget->clearHoverVisual();
    }
    _hoveringWidget = widget;
    _hoveringWidget->drawHoverVisual();    
}
