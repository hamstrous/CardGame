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

    _drawNode = ax::DrawNode::create();
    this->addChild(_drawNode);

    scheduleUpdate();
    return true;
}

void Widget::update(float delta) {
    InputManager* InputManager = InputManager::getInstance();
    InputManager->isWidgetIntersectingRubberBand(this);
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

void Widget::setSizeAndDraw(const ax::Size& size) {
    this->setContentSize(size);

    _drawNode->clear();
    _drawNode->drawRect(ax::Vec2(0, 0), ax::Vec2(size.width, size.height), ax::Color4F::BLACK, 4.f);
    _drawNode->drawPoint(Vec2(0, 0), 5.f, ax::Color4F::RED, DrawNode::PointType::Circle);

}

void Widget::drawProperties()
{
    ImGui::PushID(this->getId().c_str());
    ImGui::DragFloat("X", &this->_position.x, 0.005f);
    ImGui::DragFloat("Y", &this->_position.y, 0.005f);
    ImGui::PopID();

    // Mark the transform as dirty to ensure the changes take effect
    // This is how axmol's Node work
    _transformUpdated = _transformDirty = _inverseDirty = true;
    _usingNormalizedPosition                            = false;
}   
