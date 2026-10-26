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
    _mouseListener->setSwallowMouse(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _placeholderVisual = ax::DrawNode::create();
    this->addChild(_placeholderVisual);

    scheduleUpdate();
    return true;
}

void Widget::update(float delta) {
    InputManager* InputManager = InputManager::getInstance();
    InputManager->isWidgetIntersectingRubberBand(this);

    _placeholderVisual->clear();
    _placeholderVisual->drawRect(ax::Vec2(0, 0), this->getContentSize(), ax::Color4F::BLACK, 4.f);
    _placeholderVisual->drawPoint(this->getAnchorPointInPoints(), 5.f, ax::Color4F::RED, DrawNode::PointType::Circle);
}

bool Widget::onMouseDown(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());
    if (isWorldPositionInNode(this, mousePos))  // containPoint(this,mousePos))
    {
        AXLOGD("Widget clicked");
        InputManager::getInstance()->justClickedInsideWidget(this);
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

    ImGui::DragFloat("Scale X", &this->_scaleX, 0.5f);
    ImGui::DragFloat("Scale Y", &this->_scaleY, 0.5f);

    ImGui::SliderFloat("Anchor X", &this->_anchorPoint.x, 0.0f, 1.0f);
    ImGui::SliderFloat("Anchor Y", &this->_anchorPoint.y, 0.0f, 1.0f);

    ImGui::PopID();

    markTransformDirty();
}   
