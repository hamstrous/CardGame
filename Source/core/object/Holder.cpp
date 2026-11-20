#include "Holder.h"

#include "ImGui/ImGuiPresenter.h"

#include "utils/helper.h"
#include "utils/ScreenHelper.h"
#include "utils/DrawNodeHelper.h"
#include "utils/Collision.h"

#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include "core/singleton/InputManager.h"
#include "core/singleton/LayerManager.h"
#include "core/singleton/ModeManager.h"
#include "core/ui/GuiManager.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace helper;
using namespace ax;
using namespace std;

int Holder::_count = 0;

Holder* Holder::create()
{
    Holder* holder = new (std::nothrow) Holder();
    if (holder && holder->init())
    {
        holder->autorelease();
        return holder;
    }
    AX_SAFE_DELETE(holder);
    return nullptr;
}

// The layer that holder get added to is deterministic
void Holder::addToLayer()
{
    LayerManager::getInstance()->addNodeToNonInteractiveLayer(this);
}

bool Holder::init()
{
    if (!Widget::init())
    {
        return false;
    }  

    _shadow = Sprite::create();
    LayerManager::getInstance()->addNodeToObjectLayer(_shadow);
    _shadow->setVisible(false);

    //updateSnapPositionList();

    return true;
}

Holder* Holder::clone() const
{
    Holder* holder = Holder::create();
    holder->setContentSize(this->getContentSize());
    holder->setPosition(this->getPosition() + ax::Vec2(30, -30));
    holder->setScale(this->getScale());
    holder->setRotation(this->getRotation());
    return holder;
}

void Holder::drawGui()
{
    //ImGui::DragFloat("Corner", &_roundedCorner, 0.005f, 0.f, 1.f);
}

void Holder::update(float delta) {
    Widget::update(delta);

    if (_previouslyHoveredOn)
    {

    }
}

//void Holder::setContentSize(const ax::Size& contentSize) {
//    Node::setContentSize(contentSize);
//    if (_image) {
//        _image->setContentSize(contentSize);
//    }
//}

void Holder::setContentSize(const ax::Size& size)
{
    Node::setContentSize(size);
    this->updateSnapPositionList();
}

void Holder::setUpMainVisual()
{
    _drawNodeVisual = ax::DrawNode::create();
    _drawNodeVisual->properties.setDrawOrder(true);
    this->addChild(_drawNodeVisual);

    //_image = Sprite::create("virtualtabletop assets/cards-default/1B.svg");
    _image = Sprite::create();
    this->addChild(_image);

    schedule([this](float delta) {
        float thickness = 2.5f;
        Vec2 size       = this->getContentSize();
        
        _drawNodeVisual->clear();
        DrawNodeHelper::drawSolidCornerRect(_drawNodeVisual, size, Color4F::WHITE, Color4F::BLACK, thickness, _roundedCorner);

        //_drawNodeVisual->drawDot(this->getAnchorPointInPoints(), 5.f, ax::Color4F::RED);
    }, "placeholder_visual");
}

bool Holder::isWidgetHoveringOn(Widget* widget, Vec2& mtv)
{
    // Fast checking
    Rect box = helper::getNodeWorldBoundingBox(this);
    Rect widgetBox = helper::getNodeWorldBoundingBox(widget);

    // Remove target because maybe the previous frame widget is hovering on it

    if (_dropLimit != -1 && _dropLimit <= _children.size())
    {
        setHoverTarget(nullptr);
        return false;
    }

    if (!box.intersectsRect(widgetBox) || !Collision::overlap(this, widget, mtv))
    {
        setHoverTarget(nullptr); 
        return false;
    }
    return true;
}

void Holder::otherHoveringOnAbove(Widget* widget)
{
    setHoverTarget(widget);
    helper::spriteSetUpWithFileName(_shadow, widget->getActiveFaceFileLink(), Vec2(0.5f, 0.5f));
    _shadow->setVisible(true);
    _shadow->setContentSize(widget->getContentSize());
    // Turn dark
    _shadow->setColor(Color3B::GRAY);

    int snapIndex          = getIndexOfClosestSnapPoint(widget);

    moveOtherWidgetAside(snapIndex);

    // Move shadow to the snap
    _shadow->setWorldPosition(widget->getPositionOffsetByDirection(this->getWorldSnapPositionAtIndex(snapIndex), _spreadDirection));

}

void Holder::otherStopHoveringOnAbove(Widget* widget)
{
    setHoverTarget(nullptr);
    moveWidgetBackToPosition();
}

void Holder::onWidgetEnter(Widget* widget)
{
    int index = getIndexOfClosestSnapPoint(widget);
    moveOtherWidgetAside(index);

}

void Holder::onWidgetLeave(Widget* widget) {}

void Holder::moveWidgetBackToPosition() {}

void Holder::moveOtherWidgetAside(int snapIndex) {}

void Holder::updateSnapPositionList()
{
    // Recreate the snap list with the current state;

    auto size = this->getContentSize();

    // Set up the offset
    if (_spreadDirection == Vec2::UNIT_X) {
        // Spread to right
        _dropOffset = Vec2(0, size.height / 2);
    }
    else if (_spreadDirection == -Vec2::UNIT_X)
    {
        // Spread to left
        _dropOffset = Vec2(size.width, size.height / 2);

    }
    else if (_spreadDirection == Vec2::UNIT_Y)
    {
        // Spread to up
        _dropOffset = Vec2(size.width / 2, 0);
    }
    else if (_spreadDirection == -Vec2::UNIT_Y)
    {
        // Spread to bottom
        _dropOffset = Vec2(size.width / 2, size.height);
    }
    else if (_spreadDirection == Vec2::ZERO)
    {
        _dropOffset = size / 2;
    }
    const float offset = 50.f;
    _stackOffset = _spreadDirection * offset;

    for (int i = 0; i <= _children.size(); i++) // One extra point for future drop in widget
    {
        auto nodeSnapPoint = _dropOffset + _stackOffset * i;
        _snapPositionList.push_back(nodeSnapPoint);
    }

    moveWidgetBackToPosition();
}

std::vector<ax::Vec2> Holder::getAvailableWorldSnapPosition()
{
    vector<Vec2> worldSnapPositions;
    for (auto nodeSnapPosition : _snapPositionList)
    {
        worldSnapPositions.push_back(this->convertToWorldSpace(nodeSnapPosition));
    }

    return worldSnapPositions;
}

ax::Vec2 Holder::getWorldSnapPositionAtIndex(int index)
{
    AXASSERT(index < _snapPositionList.size(), "Index greater than snap position list size");
    return this->convertToWorldSpace(_snapPositionList[index]);
}

int Holder::getIndexOfClosestSnapPoint(Widget* widget)
{
    auto worldPosition = helper::getNodePositionInWorldSpace(widget);
    auto worldSnapPosition = getAvailableWorldSnapPosition();

    float minDistance = std::numeric_limits<float>::max();
    int minIndex      = -1;

    for (int i = 0; auto& snapPosition : worldSnapPosition)
    {
        float distance = snapPosition.distanceSquared(worldPosition);
        if (distance < minDistance)
        {
            minDistance = distance;
            minIndex    = i;
        }
        ++i;
    }

    return minIndex;
}
