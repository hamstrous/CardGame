#include "Widget.h"
#include "utils/helper.h"

#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include "core/singleton/InputManager.h"
#include "core/singleton/LayerManager.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace helper;
using namespace ax;
using namespace std;

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

// The layer that widget get added to is deterministic
void Widget::addToLayer()
{
    LayerManager::getInstance()->addWidgetToObjectLayer(this);
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

    setUpMainVisual();
    setUpInteractEffect();

    scheduleUpdate();

    _inspectorData = createInspectorData();

    this->setName(getId());

    addAllowedRotation(0);
    addAllowedRotation(90);
    addAllowedRotation(180);
    addAllowedRotation(270);

    return true;
}

Widget* Widget::clone() const
{
    Widget* widget = Widget::create();
    widget->setContentSize(this->getContentSize());
    widget->setPosition(this->getPosition() + ax::Vec2(30, -30));
    widget->setScale(this->getScale());
    widget->setRotation(this->getRotation());
    return widget;
}

void Widget::update(float delta) {
    InputManager* inputManager = InputManager::getInstance();
    inputManager->isWidgetIntersectingRubberBand(this);
}

void Widget::setUpInteractEffect() {
    this->setAnchorPoint(Vec2(0.5f, 0.5f));

    _hoverVisual = ax::DrawNode::create();
    _hoverVisual->properties.setDrawOrder(true);
    this->addChild(_hoverVisual);

    _selectedVisual = ax::DrawNode::create();
    _selectedVisual->properties.setDrawOrder(true);
    this->addChild(_selectedVisual);

    _scalableBorderVisual = ax::DrawNode::create();
    _scalableBorderVisual->properties.setDrawOrder(true);
    this->addChild(_scalableBorderVisual);

    _rotatableCircleVisual = ax::DrawNode::create();
    _rotatableCircleVisual->properties.setDrawOrder(true);
    this->addChild(_rotatableCircleVisual);

    setCascadeOpacityEnabled(true);
}

void Widget::setUpMainVisual() {
    _placeholderVisual = ax::DrawNode::create();
    _placeholderVisual->properties.setDrawOrder(true);
    this->addChild(_placeholderVisual);

    schedule([this](float delta) {
        _placeholderVisual->clear();
        _placeholderVisual->drawRect(ax::Vec2(0, 0), this->getContentSize(), ax::Color4F::BLACK, 4.f);
        _placeholderVisual->drawDot(this->getAnchorPointInPoints(), 5.f, ax::Color4F::RED);
    }, "placeholder_visual");
}

bool Widget::onMouseDown(ax::Event* event)
{
    EventMouse* e = static_cast<ax::EventMouse*>(event);

    if (e->getMouseButton() != ax::EventMouse::MouseButton::BUTTON_LEFT)
        return false;

    auto mousePos = ax::Vec2(e->getCursorX(), e->getCursorY());

    // If click on border and in select mode alone then drag
    auto borderPos = isPointOnBorder(mousePos, 10);
    if (borderPos != BorderPosition::NONE)
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

    //ImGui::DragFloat("Scale X", &this->_scaleX, 0.005f);
    //ImGui::DragFloat("Scale Y", &this->_scaleY, 0.005f);

    //ImGui::DragFloat("Anchor X", &this->_anchorPoint.x, 0.005f);
    //ImGui::DragFloat("Anchor Y", &this->_anchorPoint.y, 0.005f);

    ImGui::PopID();

    markTransformDirty();
}

InspectorData Widget::createInspectorData()
{
    InspectorData data;

    data.haveBasicData = true;
    data.position      = &this->_position;
    data.rotation      = &this->_rotationZ_X;
    data.size          = &this->_contentSize;

    return data;
}

void Widget::redrawEffect() {
    // if the visual is active then it can be redraw
    if (_hoverVisualActive)
    {
        drawHoverEffect();
    }
    if (_scalableBorderVisualActive)
    {
        drawScalableBorder();
    }
    if (_rotatableCircleVisualActive)
    {
        drawRotatableCircle();
    }
    if (_selectedVisualActive)
    {
        drawSelectedEffect();
    }
}

void Widget::clearEffect() {
    clearHoverEffect();
    clearSelectedEffect();
    clearScalableBorder();
    clearRotatableCircle();
}

void Widget::drawModifyTransformEffect()
{
    drawScalableBorder();
    drawRotatableCircle();
}

void Widget::clearModifyTransformEffect()
{
    clearScalableBorder();
    clearRotatableCircle();
}

void Widget::drawHoverEffect()
{
    if (_hoverVisual == nullptr)
        return;
    _hoverVisualActive = true;
    _hoverVisual->clear();
    Vec2 offset = Vec2(2.f, 2.f);
    _hoverVisual->drawRect(-offset, this->getContentSize() + offset, ax::Color4F::ORANGE, 2.f);
}

void Widget::clearHoverEffect()
{
    if (_hoverVisual == nullptr)
        return;
    _hoverVisualActive = false;
    _hoverVisual->clear();
}

void Widget::drawSelectedEffect() {
    if (_selectedVisual == nullptr)
        return;
    _selectedVisualActive = true;
    _selectedVisual->clear();
    Vec2 offset = Vec2(2.f, 2.f);
    _selectedVisual->drawRect(-offset, this->getContentSize() + offset, ax::Color4F::RED, 2.f);
}

void Widget::clearSelectedEffect() {
    if (_selectedVisual == nullptr)
        return;
    _selectedVisualActive = false;
    _selectedVisual->clear();
}

void Widget::drawScalableBorder() {
    if (_scalableBorderVisual == nullptr)
        return;

    _scalableBorderVisualActive = true;

    _scalableBorderVisual->clear();

    float offset = 1.f;
    auto bottomLeft   = getBorderPoint(BorderPosition::BOTTOM_LEFT, offset);
    auto bottomRight  = getBorderPoint(BorderPosition::BOTTOM_RIGHT, offset);
    auto topLeft      = getBorderPoint(BorderPosition::TOP_LEFT, offset);
    auto topRight     = getBorderPoint(BorderPosition::TOP_RIGHT, offset);
    auto leftBorder   = getBorderPoint(BorderPosition::LEFT_BORDER, offset);
    auto rightBorder  = getBorderPoint(BorderPosition::RIGHT_BORDER, offset);
    auto topBorder    = getBorderPoint(BorderPosition::TOP_BORDER, offset);
    auto bottomBorder = getBorderPoint(BorderPosition::BOTTOM_BORDER, offset);


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
    auto circleCenterLocal = _rotatableCirclePosition * Vec2(this->getContentSize().x, 1);
    _rotatableCircleVisual->drawSolidCircle(circleCenterLocal, 10.f, 0.f, 32, ax::Color4F::WHITE);
}
void Widget::clearRotatableCircle() {
    if (_rotatableCircleVisual == nullptr)
        return;
    _rotatableCircleVisualActive = false;
    _rotatableCircleVisual->clear();
}

void Widget::drawEmphasisEffect() {
    auto runningEmphasisAction = this->getActionByTag(constant::ActionTag::WIDGET_DRAG_EMPHASIS);
    if (runningEmphasisAction == nullptr)
    {
        _previousRotation = this->getRotation();
    }
    this->stopActionByTag(constant::ActionTag::WIDGET_DRAG_EMPHASIS);
    auto scaleTo = ax::ScaleTo::create(0.05f, 1.05f, 1.05f);
    auto rotateTo = ax::RotateTo::create(0.05f, _previousRotation + 2.f);
    auto action   = ax::Spawn::create(scaleTo, rotateTo, nullptr);

    action->setTag(constant::ActionTag::WIDGET_DRAG_EMPHASIS);
    this->runAction(action);
}

void Widget::clearEmphasisEffect() {
    // Reset the scale and rotation to their original values
    this->stopActionByTag(constant::ActionTag::WIDGET_ROTATION);
    this->stopActionByTag(constant::ActionTag::WIDGET_DRAG_EMPHASIS);
    auto scaleTo  = ax::ScaleTo::create(0.05f, 1.f, 1.f);
    auto rotateTo = ax::RotateTo::create(0.05f, _previousRotation);
    auto action   = ax::Spawn::create(scaleTo, rotateTo, nullptr);

    action->setTag(constant::ActionTag::WIDGET_DRAG_EMPHASIS);
    this->runAction(action);
}

void Widget::drawTransparentEffect() {
    this->setOpacity(128);  // Set opacity to 50%
}

void Widget::clearTransparentEffect() {
    this->setOpacity(255);  // Set opacity to 100%
}

void Widget::markTransformDirty()
{
    this->_rotationZ_Y = this->_rotationZ_X;
    _transformUpdated = _transformDirty = _inverseDirty = true;
    _anchorPointInPoints.set(_contentSize.width * _anchorPoint.x, _contentSize.height * _anchorPoint.y);
    updateRotationQuat();  // for updating rotation
    redrawEffect();
}

void Widget::addAllowedRotation(int rotation)
{
    if (rotation == 360 || rotation == 0)
    {
        _allowedRotation.insert(0);
        _allowedRotation.insert(-360);
        _allowedRotation.insert(360);
        return;
    }
    _allowedRotation.insert(rotation % 360);
    _allowedRotation.insert(-(360 - (rotation % 360)));
}

void Widget::eraseAllowedRotation(int rotation)
{
    if (rotation == 360 || rotation == 0)
    {
        _allowedRotation.erase(0);
        _allowedRotation.erase(-360);
        _allowedRotation.erase(360);
        return;
    }
    _allowedRotation.erase(rotation % 360);
    _allowedRotation.erase(-(360 - (rotation % 360)));
}

void Widget::rotateToNeareastAllowedRotation() {
    if (_allowedRotation.empty())
        return;
    int nearest = (*_allowedRotation.begin());
    int currentRotation = this->getRotation();
    for (auto degree : _allowedRotation)
    {
        if (abs(currentRotation - degree) < abs(currentRotation - nearest))
            nearest = degree;
    }
    auto rotateTo = ax::RotateTo::create(0.1f, nearest);
    rotateTo->setTag(constant::ActionTag::WIDGET_ROTATION);
    runAction(rotateTo);
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

    return BorderPosition::NONE;
}

bool Widget::isPointOnRotatableCircle(const Vec2& point, float threshold) const
{
    auto circleCenterLocal = _rotatableCirclePosition * Vec2(this->getContentSize().x, 1);
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
    case BorderPosition::BOTTOM_LEFT:
        point = Vec2(0, 0) - Vec2(offset, offset);
        break;
    case BorderPosition::BOTTOM_RIGHT:
        point = Vec2(width, 0) + Vec2(offset, -offset);
        break;
    case BorderPosition::TOP_RIGHT:
        point = Vec2(width, height) + Vec2(offset, offset);
        break;
    case BorderPosition::TOP_LEFT:
        point = Vec2(0, height) + Vec2(-offset, offset);
        break;
    case BorderPosition::LEFT_BORDER:
        point = Vec2(0, centerY) + Vec2(-offset, 0);
        break;
    case BorderPosition::RIGHT_BORDER:
        point = Vec2(width, centerY) + Vec2(offset, 0);
        break;
    case BorderPosition::TOP_BORDER:
        point = Vec2(centerX, height) + Vec2(0, offset);
        break;
    case BorderPosition::BOTTOM_BORDER:
        point = Vec2(centerX, 0) + Vec2(0, -offset);
        break;
    default:
        point = Vec2(0, 0);
        AXLOGD("Widget::getCornerOrBorderPoint: Invalid position enum value");
        break;
    }
    return point + Vec2(offset, offset);
}
