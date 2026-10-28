#include "Widget.h"
#include "Zone.h"
#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include "core/ui/InputManager.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace helper;

int Widget::_count = 0;

Widget* Widget::create()
{
    Widget* widget = new (std::nothrow) Widget();
    if (widget && widget->init())
    {
        widget->_id = ++_count;
        widget->autorelease();
        return widget;
    }
    AX_SAFE_DELETE(widget);
    return nullptr;
}

bool Widget::init()
{
    if (!Node::init())
    {
        return false;
    }  

    _mouseListener = ax::EventListenerMouse::create();
    _mouseListener->onMouseDown = AX_CALLBACK_1(Widget::onMouseDown, this);
    _mouseListener->onMouseMove = AX_CALLBACK_1(Widget::onMouseMove, this);
    _mouseListener->setSwallowMouse(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _placeholderVisual = ax::DrawNode::create();
    _placeholderVisual->properties.setDrawOrder(true);
    this->addChild(_placeholderVisual);

    _hoverVisual = ax::DrawNode::create();
    _hoverVisual->properties.setDrawOrder(true);
    this->addChild(_hoverVisual);

    _scalableBorderVisual = ax::DrawNode::create();
    _scalableBorderVisual->properties.setDrawOrder(true);
    this->addChild(_scalableBorderVisual);

    _rotatableCircleVisual = ax::DrawNode::create();
    _rotatableCircleVisual->properties.setDrawOrder(true);
    this->addChild(_rotatableCircleVisual);

    scheduleUpdate();
    return true;
}

void Widget::update(float delta) {
    InputManager* inputManager = InputManager::getInstance();
    inputManager->isWidgetIntersectingRubberBand(this);

    _placeholderVisual->clear();
    _placeholderVisual->drawRect(ax::Vec2(0, 0), this->getContentSize(), ax::Color4F::BLACK, 4.f);
    _placeholderVisual->drawDot(this->getAnchorPointInPoints(), 5.f, ax::Color4F::RED);
    
}

bool Widget::onMouseDown(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);

    if (e->getMouseButton() != ax::EventMouse::MouseButton::BUTTON_LEFT)
        return false;

    auto mousePos = ax::Vec2(e->getCursorX(), e->getCursorY());

    // If click on border and in select mode alone then drag
    auto borderPos = isPointOnBorder(mousePos, 10);
    if (borderPos != BorderPosition::None)
    {
        if(InputManager::getInstance()->justClickedOnWidgetBorder(this, _borderPointsNormalized[borderPos]))
            return true;
    }

    // If click on rotatable circle and in select mode alone then rotate
    if (isPointOnRotatableCircle(mousePos, 10))
    {
        if (InputManager::getInstance()->justClickedOnWidgetCircle(this))
            return true;
    }

    if (isWorldPositionInNode(this, mousePos))  // containPoint(this,mousePos))
    {
        //AXLOGD("Widget clicked");
        InputManager::getInstance()->justClickedInsideWidget(this);
        return true;  // Event swallowed
    }
    return false;  // Propagate to other listeners
}

bool Widget::onMouseMove(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());
    if (isWorldPositionInNode(this, mousePos))  // containPoint(this,mousePos))
    {
        InputManager::getInstance()->hoveringInsideWidget(this);
        return true;  // Event swallowed
    }
    return false;  // Propagate to other listeners
}


void Widget::drawProperties()
{
    ImGui::PushID(this->getId().c_str());
    ImGui::DragFloat("X", &this->_position.x, 0.5f);
    ImGui::DragFloat("Y", &this->_position.y, 0.5f);

    ImGui::DragFloat("Width", &this->_contentSize.width, 0.5f);
    ImGui::DragFloat("Height", &this->_contentSize.height, 0.5f);

    ImGui::DragFloat("Rotation X", &this->_rotationX, 0.5f);
    ImGui::DragFloat("Rotation Y", &this->_rotationY, 0.5f);
    ImGui::DragFloat("Rotation Z", &this->_rotationZ_X, 0.5f);

    ImGui::DragFloat("Scale X", &this->_scaleX, 0.005f);
    ImGui::DragFloat("Scale Y", &this->_scaleY, 0.005f);

    ImGui::DragFloat("Anchor X", &this->_anchorPoint.x, 0.005f);
    ImGui::DragFloat("Anchor Y", &this->_anchorPoint.y, 0.005f);

    ImGui::PopID();

    markTransformDirty();
}

void Widget::redrawVisual() {
    if (_hoverVisualActive)
    {
        drawHoverVisual();
    }
    if (_scalableBorderVisualActive)
    {
        drawScalableBorder();
    }
    if (_rotatableCircleVisualActive)
    {
        drawRotatableCircle();
    }
}   

void Widget::drawHoverVisual()
{
    if (_hoverVisual == nullptr)
        return;
    _hoverVisualActive = true;
    _hoverVisual->clear();
    Vec2 offset = Vec2(2.f, 2.f);
    _hoverVisual->drawRect(-offset, this->getContentSize() + offset, ax::Color4F::RED, 2.f);
}

void Widget::clearHoverVisual()
{
    if (_hoverVisual == nullptr)
        return;
    _hoverVisualActive = false;
    _hoverVisual->clear();
}

void Widget::drawScalableBorder() {
    if (_scalableBorderVisual == nullptr)
        return;

    _scalableBorderVisualActive = true;

    _scalableBorderVisual->clear();

    float offset = 1.f;
    auto bottomLeft   = getBorderPoint(BorderPosition::BottomLeft, offset);
    auto bottomRight  = getBorderPoint(BorderPosition::BottomRight, offset);
    auto topLeft      = getBorderPoint(BorderPosition::TopLeft, offset);
    auto topRight     = getBorderPoint(BorderPosition::TopRight, offset);
    auto leftBorder   = getBorderPoint(BorderPosition::LeftBorder, offset);
    auto rightBorder  = getBorderPoint(BorderPosition::RightBorder, offset);
    auto topBorder    = getBorderPoint(BorderPosition::TopBorder, offset);
    auto bottomBorder = getBorderPoint(BorderPosition::BottomBorder, offset);


    float dotSize = 4.f;
    // Draw a dot in 4 corners
    _scalableBorderVisual->drawDot(bottomLeft, dotSize, ax::Color4F::WHITE);
    _scalableBorderVisual->drawDot(bottomRight, dotSize, ax::Color4F::WHITE);
    _scalableBorderVisual->drawDot(topLeft, dotSize, ax::Color4F::WHITE);
    _scalableBorderVisual->drawDot(topRight, dotSize, ax::Color4F::WHITE);

    // Draw a short line in the middle of each edge
    float lineLength = 5.f;
    const float halfLineLength = lineLength / 2.f;
    float width                = this->getContentSize().width;
    float height               = this->getContentSize().height;
    float centerX              = width / 2.f;
    float centerY              = height / 2.f;
    _scalableBorderVisual->drawLine(leftBorder - Vec2(0, halfLineLength), leftBorder + Vec2(0, halfLineLength),
                                    ax::Color4F::WHITE, 8.f);
    _scalableBorderVisual->drawLine(topBorder - Vec2(halfLineLength, 0), topBorder + Vec2(halfLineLength, 0),
                                    ax::Color4F::WHITE, 8.f);
    _scalableBorderVisual->drawLine(rightBorder - Vec2(0, halfLineLength), rightBorder + Vec2(0, halfLineLength),
                                    ax::Color4F::WHITE, 8.f);
    _scalableBorderVisual->drawLine(bottomBorder - Vec2(halfLineLength, 0), bottomBorder + Vec2(halfLineLength, 0),
                                    ax::Color4F::WHITE, 8.f);
}

void Widget::clearScalableBorder()
{
    if (_scalableBorderVisual == nullptr)
        return;
    _scalableBorderVisualActive = false;
    _scalableBorderVisual->clear();
}

void Widget::drawRotatableCircle() {
    if (_rotatableCircleVisual == nullptr)
        return;
    _rotatableCircleVisualActive = true;

    _rotatableCircleVisual->clear();
    auto circleCenterLocal = _rotatableCircleNormalized * this->getContentSize();
    _rotatableCircleVisual->drawSolidCircle(circleCenterLocal, 10.f, 0.f, 32, ax::Color4F::WHITE);
}
void Widget::clearRotatableCircle() {
    if (_rotatableCircleVisual == nullptr)
        return;
    _rotatableCircleVisualActive = false;
    _rotatableCircleVisual->clear();
}

Widget::BorderPosition Widget::isPointOnBorder(const Vec2& point, float threshold) const
{
    float width = this->getContentSize().width;
    float height = this->getContentSize().height;

    for (auto& [position, normalizedPoint] : _borderPointsNormalized)
    {
        Vec2 borderPointLocal = Vec2(normalizedPoint.x * width, normalizedPoint.y * height);
        Vec2 borderPoint      = this->convertToWorldSpace(borderPointLocal);

        if (point.distance(borderPoint) <= threshold)
        {
            return position;
        }
    }

    return BorderPosition::None;
}

bool Widget::isPointOnRotatableCircle(const Vec2& point, float threshold) const
{
    auto circleCenterLocal = _rotatableCircleNormalized * this->getContentSize();
    Vec2 circleCenter = this->convertToWorldSpace(circleCenterLocal);
    return point.distance(circleCenter) <= threshold;
}

Vec2 Widget::getBorderPoint(BorderPosition position, float offset) const
{
    Vec2 point;
    float width   = this->getContentSize().width;
    float height  = this->getContentSize().height;
    float centerX = width / 2.f;
    float centerY = height / 2.f;
    switch (position)
    {
    case BorderPosition::BottomLeft:
        point = Vec2(0, 0) - Vec2(offset, offset);
        break;
    case BorderPosition::BottomRight:
        point = Vec2(width, 0) + Vec2(offset, -offset);
        break;
    case BorderPosition::TopRight:
        point = Vec2(width, height) + Vec2(offset, offset);
        break;
    case BorderPosition::TopLeft:
        point = Vec2(0, height) + Vec2(-offset, offset);
        break;
    case BorderPosition::LeftBorder:
        point = Vec2(0, centerY) + Vec2(-offset, 0);
        break;
    case BorderPosition::RightBorder:
        point = Vec2(width, centerY) + Vec2(offset, 0);
        break;
    case BorderPosition::TopBorder:
        point = Vec2(centerX, height) + Vec2(0, offset);
        break;
    case BorderPosition::BottomBorder:
        point = Vec2(centerX, 0) + Vec2(0, -offset);
        break;
    default:
        point = Vec2(0, 0);
        AXLOGD("Widget::getCornerOrBorderPoint: Invalid position enum value");
        break;
    }
    return point + Vec2(offset, offset);
}
