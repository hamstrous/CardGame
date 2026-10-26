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
    _mouseAfterListener->onMouseMove = AX_CALLBACK_1(InputManager::onMouseMove, this);
    _mouseAfterListener->onMouseUp = AX_CALLBACK_1(InputManager::onMouseUp, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_mouseAfterListener, 1);  // get called after scene graph


    _mouseBeforeListener = ax::EventListenerMouse::create();
    _mouseBeforeListener->onMouseDown = AX_CALLBACK_1(InputManager::onMouseDownBefore, this);
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

bool InputManager::onMouseMove(Event* event)
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

    _mousePreviousPosition = mousePos;

    return false;
}

bool InputManager::onMouseUp(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    auto mousePos   = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    
    endRubberBandSelection(mousePos);
    endDragging();

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
