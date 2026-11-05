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

    _modeListener = EventListenerMode::create();
    _modeListener->onModeChanged = AX_CALLBACK_1(InputManager::onModeChanged, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_modeListener, 1);

    _layerManager = LayerManager::getInstance();

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
    // no widget got clicked
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    auto mousePos   = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    _justClickedInsideWidget = false;

    if (_dragging)
    {
        return false;
    }

    if(_canRubberband)
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

    if(isRotating())
    {
        updateRotating(mousePos);
    }
    
    _mousePreviousPosition = mousePos;

    return false;
}

bool InputManager::onMouseMoveAfter(Event* event) {
    // Not swallowed, mean hovering over nothing
    if (_hoveringWidget != nullptr)
    {
        if (_selectedWidgets.find(_hoveringWidget) == _selectedWidgets.end())
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
    endRotating();

    return false;
}

bool InputManager::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    _pressingKeys.insert(keyCode);
    if (keyCode == EventKeyboard::KeyCode::KEY_C)
    {
        duplicateSelectedWidgets();
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        deleteSelectedWidgets();
    }
    return false;
}

bool InputManager::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    _pressingKeys.erase(keyCode);
    return false;
}

bool InputManager::onModeChanged(EventMode* eventMode)
{
    Mode newMode = eventMode->getMode();
    _justClickedInsideWidget = false;
    _dragging                = false;
    _scaling                 = false;
    _rotating                = false;
    _rubberBanding           = false;
    clearSelectedWidgets();
    if (newMode == Mode::EDIT)
    {
        _canDrag         = true;
        _canScale        = true;
        _canRotate       = true;
        _canRubberband   = true;
        _toggleSelection = false;
        _canEnlarge      = false;
    }
    else if (newMode == Mode::PLAY)
    {
        _canDrag         = true;
        _canScale        = false;
        _canRotate       = false;
        _canRubberband   = false;
        _toggleSelection = false;
        _canEnlarge      = false;
    }
    else if (newMode == Mode::SELECT)
    {
        _canDrag         = false;
        _canScale        = false;
        _canRotate       = false;
        _canRubberband   = false;
        _toggleSelection = true;
        _canEnlarge      = false;
    }
    return false;
}

void InputManager::startRubberBandSelection(const Vec2& startPoint) {
    if (!_canRubberband)
        return;

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

void InputManager::updateRubberBandSelection(const Vec2& mousePosition) {
    if (!_canRubberband || !_rubberBanding)
        return;

    _drawNode->clear();
    _drawNode->drawRect(_startPoint, mousePosition, Color4F::BLUE);
}

void InputManager::startDragging() {
    if (!_canDrag)
        return;
    _dragging = true;
}

void InputManager::endDragging() {
    _dragging = false;
}

void InputManager::updateDragging(const Vec2& mousePosition) {
    if (!_canDrag || !_dragging)
        return;
    // Move every selected widget by the delta of the mouse movement
    Vec2 delta = mousePosition - _mousePreviousPosition;
    for (auto widget : _selectedWidgets)
    {
        widget->setPosition(widget->getPosition() + delta);
    }
}

void InputManager::startScaling(const Vec2& normalizedBorderPoint) {
    if (!_canScale)
        return;

    AXASSERT(_selectedAloneWidget != nullptr, "Scaling widget is null");
    _scaling = true;
    _normalizedBorderPoint = normalizedBorderPoint;
    _previousAnchorPoint   = _selectedAloneWidget->getAnchorPoint();
    //_scalingWidget->setAnchorPoint(Vec2(1, 1) - _normalizedBorderPoint); // Set anchor point to the opposite of the border point

    setNodeAnchorPointPreservePosition(_selectedAloneWidget, Vec2(1, 1) - _normalizedBorderPoint);

    // Because setting anchor point will change the position of the widget (visually), we need to adjust
    //_scalingWidget->setPosition(_scalingWidget->getPosition() - (_normalizedBorderPoint - _previousAnchorPoint) * _scalingWidget->getContentSize());
}

void InputManager::endScaling() {
    if (!_scaling)
        return;
    _scaling = false;
    setNodeAnchorPointPreservePosition(_selectedAloneWidget, _previousAnchorPoint);
    _selectedAloneWidget = nullptr;
}

void InputManager::updateScaling(const Vec2& mousePosition) {
    if (!_canScale || !_scaling)
        return;

    Size size = _selectedAloneWidget->getContentSize();
    float width = _selectedAloneWidget->getContentSize().width;
    float height = _selectedAloneWidget->getContentSize().height;
    Vec2 anchorPoint  = _selectedAloneWidget->getAnchorPoint();
    Vec2 anchorPointInPoints = _selectedAloneWidget->getAnchorPointInPoints();
    Vec2 worldAnchorPoint    = _selectedAloneWidget->convertToWorldSpace(anchorPointInPoints);
    Vec2 opositeAnchorPoint = Vec2(1, 1) - _normalizedBorderPoint;
    Vec2 normalizedBorderPointInPoints = _normalizedBorderPoint * size;

    // Get the line passing through the diagonal
    // Scaling with ratio means the mouse position projected perpendicular to the diagonal line
    Vec2 pointA   = _selectedAloneWidget->convertToWorldSpace(opositeAnchorPoint * size);
    Vec2 pointB   = _selectedAloneWidget->convertToWorldSpace(_normalizedBorderPoint * size);
    Vec2 vectorAB = pointB - pointA;
    Vec3 line   = getLineEquationFromTwoPoints(pointA, pointB);

    Vec2 perpendicularIntersectedPoint = getSolutionForLineIntersection(
        line, Vec3(vectorAB.x, vectorAB.y, -(vectorAB.x * mousePosition.x + vectorAB.y * mousePosition.y))
    );
    Vec2 perpendicularIntersectedPointInNodeSpace =
        _selectedAloneWidget->convertToNodeSpace(perpendicularIntersectedPoint);

    Vec2 delta = perpendicularIntersectedPointInNodeSpace - normalizedBorderPointInPoints;
    _selectedAloneWidget->convertToNodeSpace(delta);

    delta = Vec2(delta.x, delta.y);

    if (anchorPoint.x > 0.5f)
    {
        delta.x = -delta.x;
    }
    if (anchorPoint.y > 0.5f)
    {
        delta.y = -delta.y;
    }
    _selectedAloneWidget->setContentSize(Size(width, height) + delta);
    
}

void InputManager::startRotating() {
    if (!_canRotate)
        return;

    _rotating = true;
    _previousAnchorPoint = _selectedAloneWidget->getAnchorPoint();
    setNodeAnchorPointPreservePosition(_selectedAloneWidget, Vec2(0.5f, 0.5f));
}

void InputManager::endRotating() {
    if (!_rotating)
        return;
    _rotating = false;
    setNodeAnchorPointPreservePosition(_selectedAloneWidget, _previousAnchorPoint);
    _selectedAloneWidget = nullptr;
}

void InputManager::updateRotating(const Vec2& mousePosition) {
    if (!_canRotate || !_rotating)
        return;

    // anchor should be in the center
    Vec2 widgetCenter = _selectedAloneWidget->getAnchorPointInPoints();
    Vec2 mousePositionInNodeSpace = _selectedAloneWidget->convertToNodeSpace(mousePosition);
    Vec2 previousMousePositionInNodeSpace = _selectedAloneWidget->convertToNodeSpace(_mousePreviousPosition);

    Vec2 vectorA = mousePositionInNodeSpace - widgetCenter;
    Vec2 vectorB = previousMousePositionInNodeSpace - widgetCenter;

    float offsetAngleinRad = vectorA.getAngle(vectorB);
    float offsetAngleinDeg = AX_RADIANS_TO_DEGREES(offsetAngleinRad);

    _selectedAloneWidget->setRotation(_selectedAloneWidget->getRotation() + offsetAngleinDeg);
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

void InputManager::CheckAloneWidget()
{
    if (_selectedWidgets.size() == 1)
    {
        (*_selectedWidgets.begin())->drawScalableBorder();
        (*_selectedWidgets.begin())->drawRotatableCircle();
    }
    else
    {
        for (auto widget : _selectedWidgets)
        {
            widget->clearTransformModifyVisual();
        }
    }
}

void InputManager::duplicateSelectedWidgets() {
    _layerManager->duplicateWidgets(_selectedWidgets);
}

void InputManager::deleteSelectedWidgets()
{
    _layerManager->deleteWidgets(_selectedWidgets);
    // Dont use the clearSelectedWidgets() function here,
    // because it will clear the visuals of the widgets, which are already deleted
    if (isWidgetSelected(_selectedAloneWidget))
    {
        _selectedAloneWidget = nullptr;
    }
    if (isWidgetSelected(_hoveringWidget))
    {
        _hoveringWidget = nullptr;
    }
    _selectedWidgets.clear();  
}

void InputManager::justClickedInsideWidget(Widget* widget)
{
    _layerManager->moveWidgetToFront(widget);

    _justClickedInsideWidget = true;

    // If click on the same set of selected widgets
    // Then we can drag all
    // Else this is the new selected widget

    bool isShiftPressed = _pressingKeys.find(EventKeyboard::KeyCode::KEY_SHIFT) != _pressingKeys.end();
    bool widgetSelected = isWidgetSelected(widget);
    if ((_toggleSelection || isShiftPressed) && widgetSelected)
    {
        removeSelectedWidget(widget);
    }
    else if ((_toggleSelection || isShiftPressed) && !widgetSelected)
    {
        addSelectedWidget(widget);
    }
    else if (!widgetSelected)
    {
        clearSelectedWidgets();
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

    _selectedAloneWidget = widget;
    startScaling(normalizedBorderPoint);
    return true;
}

bool InputManager::justClickedOnWidgetCircle(Widget* widget)
{
    // Widget must be selected to scale it
    if (_selectedWidgets.find(widget) == _selectedWidgets.end())
        return false;
    // Currently we only support scaling one widget at a time
    if (_selectedWidgets.size() != 1)
        return false;

    _selectedAloneWidget = widget;
    startRotating();
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
