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
    this->addChild(_placeholderVisual);

    _hoverVisual = ax::DrawNode::create();
    this->addChild(_hoverVisual);
    _scalableBorderVisual = ax::DrawNode::create();
    this->addChild(_scalableBorderVisual);

    scheduleUpdate();
    return true;
}

void Widget::update(float delta) {
    InputManager* inputManager = InputManager::getInstance();
    inputManager->isWidgetIntersectingRubberBand(this);

    _placeholderVisual->clear();
    _placeholderVisual->drawRect(ax::Vec2(0, 0), this->getContentSize(), ax::Color4F::BLACK, 4.f);
    _placeholderVisual->drawPoint(this->getAnchorPointInPoints(), 5.f, ax::Color4F::RED, DrawNode::PointType::Circle);
    
}

bool Widget::onMouseDown(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);

    if (e->getMouseButton() != ax::EventMouse::MouseButton::BUTTON_LEFT)
        return false;

    auto mousePos = ax::Vec2(e->getCursorX(), e->getCursorY());
    if (isWorldPositionInNode(this, mousePos))  // containPoint(this,mousePos))
    {
        AXLOGD("Widget clicked");
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
    float dotSize = 8.f;
    // Draw a dot in 4 corners
    _scalableBorderVisual->drawPoint(Vec2(0, 0), dotSize, ax::Color4F::WHITE, DrawNode::PointType::Circle);
    _scalableBorderVisual->drawPoint(Vec2(this->getContentSize().width, 0), dotSize, ax::Color4F::WHITE, DrawNode::PointType::Circle);
    _scalableBorderVisual->drawPoint(Vec2(0, this->getContentSize().height), dotSize, ax::Color4F::WHITE, DrawNode::PointType::Circle);
    _scalableBorderVisual->drawPoint(Vec2(this->getContentSize().width, this->getContentSize().height), dotSize, ax::Color4F::WHITE, DrawNode::PointType::Circle);

    // Draw a short line in the middle of each edge
    float lineLength = 5.f;
    const float halfLineLength = lineLength / 2.f;
    float width                = this->getContentSize().width;
    float height               = this->getContentSize().height;
    float centerX              = width / 2.f;
    float centerY              = height / 2.f;
    _scalableBorderVisual->drawLine(Vec2(centerX - halfLineLength, 0), Vec2(centerX + halfLineLength, 0),
                                    ax::Color4F::WHITE, 8.f);
    _scalableBorderVisual->drawLine(Vec2(0, centerY - halfLineLength), Vec2(0, centerY + halfLineLength),
                                    ax::Color4F::WHITE, 8.f);
    _scalableBorderVisual->drawLine(Vec2(centerX - halfLineLength, height), Vec2(centerX + halfLineLength, height),
                                    ax::Color4F::WHITE, 8.f);
    _scalableBorderVisual->drawLine(Vec2(width, centerY - halfLineLength), Vec2(width, centerY + halfLineLength),
                                    ax::Color4F::WHITE, 8.f);
}

void Widget::clearScalableBorder()
{
    if (_scalableBorderVisual == nullptr)
        return;
    _scalableBorderVisualActive = false;
    _scalableBorderVisual->clear();
}
