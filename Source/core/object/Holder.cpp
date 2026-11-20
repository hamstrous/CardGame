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

    _widgetHolderActionTag = helper::enumToNum(constant::ActionTag::WIDGET_IN_HOLDER);

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

// Just checking to see if widget is hovering
bool Holder::isWidgetHoveringOn(Widget* widget, Vec2& mtv)
{
    AXLOGD("Checking if widget hovering on holder");

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
    AXLOGD("Widget hovering on holder");

    setHoverTarget(widget);
    helper::spriteSetUpWithFileName(_shadow, widget->getActiveFaceFileLink(), Vec2(0.5f, 0.5f));
    _shadow->setVisible(true);
    _shadow->setContentSize(widget->getContentSize());
    // Turn dark
    _shadow->setColor(Color3B::GRAY);

    int snapIndex     = getIndexOfClosestSnapPoint(widget);
    auto layerManager = LayerManager::getInstance();

    if (snapIndex != _currentHoveringIndex)
    {
        if (_currentHoveringIndex == -1)
        {
            //Just hovered recently
            // Move all children widget to front
            layerManager->moveWidgetsToFront(_children);
            layerManager->moveWidgetToFront(widget);
            // Temporary move widget layer order to last for easy layer handle
            _currentHoveringIndex = _snapPositionList.size();
            
        }
        this->stopActionByTag(_widgetHolderActionTag);
        moveOtherWidgetsFront(snapIndex);
        moveOtherWidgetsBack(snapIndex - 1);
        // Move widget order one by one
        while (_currentHoveringIndex != snapIndex)
        {
            if (_currentHoveringIndex < snapIndex)
            {
                layerManager->moveWidgetForward(widget);
                _currentHoveringIndex++;
            }
            else
            {
                layerManager->moveWidgetBackward(widget);
                _currentHoveringIndex--;
            }
        }
    }

    // Move shadow to the snap
    _shadow->setWorldPosition(getClosestWorldSnapPointWithWidgetOffset(widget));

}

void Holder::otherStopHoveringOnAbove(Widget* widget)
{
    AXLOGD("Widget stop hovering on holder");

    setHoverTarget(nullptr);
    moveWidgetBackToPosition();
    _currentHoveringIndex = -1;
    _shadow->setVisible(false);
}

void Holder::onWidgetEnter(Widget* widget)
{
    AXLOGD("Widget dropped on holder");

    int index = getIndexOfClosestSnapPoint(widget);
    this->stopActionByTag(_widgetHolderActionTag);
    moveOtherWidgetsFront(index);

    // Add widget
    if (_children.size() != _childrenWithOrder.size())
    {
        AXLOGD("MIX MATCH SIZE---------------------");
    }

    addWidgetChildren(widget);
    _childrenWithOrder.insert(_childrenWithOrder.begin() + index, widget);
    
    AXLOGD("Children size: {}; Children with order size: {}", _children.size(), _childrenWithOrder.size());
    if (_children.size() != _childrenWithOrder.size())
    {
        AXLOGD("MIX MATCH SIZE---------------------");

    }
    // Move widget to
    _shadow->setVisible(false);
    MoveTo* widgetMoveToAction = MoveTo::create(
        0.2f, helper::convertWorldPositionToParentSpace(widget, getClosestWorldSnapPointWithWidgetOffset(widget)));
    TargetedAction* targertedAction = TargetedAction::create(widget, widgetMoveToAction);
    targertedAction->setFlags(_widgetHolderActionTag);
    this->runAction(targertedAction);

    updateSnapPositionList();
}

void Holder::onWidgetLeave(Widget* widget)
{
    AXLOGD("Widget leave holder");

    std::erase(_childrenWithOrder, widget);
    removeWidgetChildren(widget);
    AXLOGD("Children size: {}; Children with order size: {}", _children.size(), _childrenWithOrder.size());
    updateSnapPositionList();
    moveWidgetBackToPosition();
}

void Holder::moveWidgetBackToPosition()
{
    Vector<FiniteTimeAction*> targetedActionList;
    if (_childrenWithOrder.empty())
    {
        return;
    }
    for (int i = 0; i < _childrenWithOrder.size(); i++)
    {
        auto widget         = _childrenWithOrder[i];
        auto newPosition    = getSnapPositionInNodeParentSpaceAtIndex(widget, i);
        auto moveToAction   = MoveTo::create(0.2f, newPosition);
        auto targetedAction = TargetedAction::create(widget, moveToAction);
        targetedActionList.pushBack(targetedAction);
    }

    // Combine it to one and run it with holder
    auto moveWidgetsAsideAction = Spawn::create(targetedActionList);
    moveWidgetsAsideAction->setTag(_widgetHolderActionTag);
    this->runAction(moveWidgetsAsideAction);
}

void Holder::moveOtherWidgetsFront(int snapIndex)
{
    AXASSERT(_snapPositionList.size() == _childrenWithOrder.size() + 1, "Snap position list not correct to move widgets aside");

    if (_childrenWithOrder.empty() || snapIndex >= _childrenWithOrder.size())
    {
        return;
    }

    // Get list of target action on each widget
    Vector<FiniteTimeAction*> targetedActionList;
    for (int i = snapIndex; i < _childrenWithOrder.size(); i++)
    {
        auto widget = _childrenWithOrder[i];
        auto newPosition = getSnapPositionInNodeParentSpaceAtIndex(widget, i + 1);
        auto moveToAction = MoveTo::create(0.2f, newPosition);
        auto targetedAction = TargetedAction::create(widget, moveToAction);
        targetedActionList.pushBack(targetedAction);
    }

    // Combine it to one and run it with holder
    auto moveWidgetsAsideAction = Spawn::create(targetedActionList);
    moveWidgetsAsideAction->setTag(_widgetHolderActionTag);
    this->runAction(moveWidgetsAsideAction);
}

void Holder::moveOtherWidgetsBack(int snapIndex)
{
    AXASSERT(_snapPositionList.size() == _childrenWithOrder.size() + 1,
             "Snap position list not correct to move widgets aside");

    if (_childrenWithOrder.empty() || snapIndex < 0)
    {
        return;
    }

    // Get list of target action on each widget
    Vector<FiniteTimeAction*> targetedActionList;
    for (int i = 0; i <= snapIndex; i++)
    {
        auto widget         = _childrenWithOrder[i];
        auto newPosition    = getSnapPositionInNodeParentSpaceAtIndex(widget, i);
        auto moveToAction   = MoveTo::create(0.2f, newPosition);
        auto targetedAction = TargetedAction::create(widget, moveToAction);
        targetedActionList.pushBack(targetedAction);
    }

    // Combine it to one and run it with holder
    auto moveWidgetsAsideAction = Spawn::create(targetedActionList);
    moveWidgetsAsideAction->setTag(_widgetHolderActionTag);
    this->runAction(moveWidgetsAsideAction);
}

void Holder::updateSnapPositionList()
{
    AXLOGD("Update snap position list");

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
    _snapPositionList.clear();
    for (int i = 0; i <= _children.size(); i++) // One extra point for future drop in widget
    {
        auto nodeSnapPoint = _dropOffset + _stackOffset * i;
        _snapPositionList.push_back(nodeSnapPoint);
    }

    AXLOGD("Updated snap list: snap count {}, children of holder count {}", _snapPositionList.size(), _childrenWithOrder.size());
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
    if (_spreadDirection == Vec2(0, 0))
        return _snapPositionList.size() - 1;

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

ax::Vec2 Holder::getClosestWorldSnapPointWithWidgetOffset(Widget* widget)
{
    return widget->getPositionOffsetByDirection(this->getWorldSnapPositionAtIndex(this->getIndexOfClosestSnapPoint(widget)), _spreadDirection);
}

ax::Vec2 Holder::getSnapPositionInNodeParentSpaceAtIndex(Widget* widget, int index)
{
    auto worldSnapPosition = widget->getPositionOffsetByDirection(this->getWorldSnapPositionAtIndex(index), _spreadDirection);
    auto ret               = helper::convertWorldPositionToParentSpace(widget, worldSnapPosition);

    return ret;
}
