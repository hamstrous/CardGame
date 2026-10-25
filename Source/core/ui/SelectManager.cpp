#include "SelectManager.h"

SelectManager* SelectManager::_instance = nullptr;

SelectManager::SelectManager()
{
    _drawNode = ax::DrawNode::create();
    _drawNode->setVisible(false);
    this->addChild(_drawNode);

    _startPoint = Vec2::ZERO;

    _mouseListener = ax::EventListenerMouse::create();
    _mouseListener->onMouseDown = AX_CALLBACK_1(SelectManager::onMouseDown, this);
    _mouseListener->onMouseMove = AX_CALLBACK_1(SelectManager::onMouseMove, this);
    _mouseListener->onMouseUp = AX_CALLBACK_1(SelectManager::onMouseUp, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_mouseListener, 1); // get called first
}

SelectManager* SelectManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new SelectManager();
    }
    return _instance;
}

void SelectManager::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }   
}

bool SelectManager::onMouseDown(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    _startPoint     = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());

    _drawNode->clear();
    _drawNode->setVisible(true);
    _drawNode->drawRect(_startPoint, _startPoint, Color4F::BLUE);

    return false;
}

bool SelectManager::onMouseMove(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    Vec2 currentPoint     = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());

    _drawNode->clear();
    _drawNode->drawRect(_startPoint, currentPoint, Color4F::BLUE);

    return false;
}

bool SelectManager::onMouseUp(Event* event)
{
    auto mouseEvent = static_cast<ax::EventMouse*>(event);
    Vec2 endPoint   = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
    _drawNode->clear();
    _drawNode->setVisible(false);

    return false;
}
