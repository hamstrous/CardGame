#include "LayerManager.h"

#include "utils/ScreenHelper.h"

#include "InputManager.h"
#include "ModeManager.h"
#include "ImGui/ImGuiPresenter.h"

#include "core/object/Widget.h"
#include "core/object/Holder.h"
#include "core/const/GameConstants.h"
#include "utils/helper.h"

#include "core/event/EventMode.h"
#include "core/event/EventListenerMode.h"

USING_NS_AX_EXT;
using namespace ax;
using namespace std;
using namespace constant;

using namespace helper;

LayerManager* LayerManager::_instance = nullptr;

LayerManager::LayerManager()
{
    //_customListener = EventListenerCustom::create("on_widget_hover", AX_CALLBACK_1(LayerManager::onWidgetHovered, this));
    _eventDispatcher->addCustomEventListener("on_widget_hover", AX_CALLBACK_1(LayerManager::onWidgetHover, this));
    _eventDispatcher->addCustomEventListener("on_widget_stop_hover", AX_CALLBACK_1(LayerManager::onWidgetStopHover, this));

    _modeListener = EventListenerMode::create();
    _modeListener->onModeChanged = AX_CALLBACK_1(LayerManager::onModeChanged, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_modeListener, 1);

    _mouseListener = EventListenerMouse::create();
    _mouseListener->onMouseDown = AX_CALLBACK_1(LayerManager::onMouseDown, this);
    _mouseListener->onMouseUp = AX_CALLBACK_1(LayerManager::onMouseUp, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_mouseListener, -1);

    _objectLayer = Node::create();
    _collectionLayer = Node::create();
    _interactiveLayer = Node::create();
    _nonInteractiveLayer = Node::create();

    this->addChild(_objectLayer, 3);
    this->addChild(_collectionLayer, 2);
    this->addChild(_interactiveLayer, 1);
    this->addChild(_nonInteractiveLayer, 0);

    _layerList.push_back(_objectLayer);
    _layerList.push_back(_collectionLayer);
    _layerList.push_back(_interactiveLayer);
    _layerList.push_back(_nonInteractiveLayer);

    _enlargeSprite = Sprite::create();
    _enlargeSprite->setGlobalZOrder(helper::enumToNum(constant::ZOrder::TOP_MOST));
    _enlargeSprite->setVisible(false);
    this->addChild(_enlargeSprite);
}

LayerManager* LayerManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new LayerManager();
    }
    return _instance;
}

void LayerManager::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }   
}

void LayerManager::onWidgetHover(ax::EventCustom* event)
{
    Widget* hoveredWidget = static_cast<Widget*>(event->getUserData());

    _hoverWidget = hoveredWidget;

    stopEnlargeWidget();
    _enlarging = true;
    scheduleOnce([=](float delta) { enlargeWidget(hoveredWidget); }, 1.f, "enlarge_widget");
}

void LayerManager::onWidgetStopHover(ax::EventCustom* event)
{
    Widget* stopHoveredWidget = static_cast<Widget*>(event->getUserData());
    stopEnlargeWidget();
    _hoverWidget = nullptr;
    _enlarging   = false;
}

bool LayerManager::onModeChanged(EventMode* event)
{
    return false;
}

bool LayerManager::onMouseDown(Event* event)
{
    if (_enlarging)
        disruptEnlargeWidget();
    return false;
}

bool LayerManager::onMouseUp(Event* event)
{
    if (_enlarging) //Disrupted and still hover same widget
    {
        scheduleOnce([=](float delta) { enlargeWidget(_hoverWidget); }, 1.f, "enlarge_widget");
    }
    return false;
}

void LayerManager::enlargeWidget(Widget* widget)
{
    if (_hoverWidget != widget)
    {
        AXLOGD("Enlarge widget different from hover widget");
        return;
    }

    // Only for play mode
    if (widget->getEnlargeRatio() == 0 || ModeManager::getInstance()->getCurrentMode() != Mode::PLAY || !widget->haveActiveFace())
        return;

    helper::spriteSetUpWithFileName(_enlargeSprite, widget->getActiveFaceFileLink(), Vec2(0.5, 0.5f));

    ScreenHelper::getWorldPositionForEnlargePreview(widget, _enlargeSprite, widget->getEnlargeRatio());
    _enlargeSprite->setVisible(true);
}

void LayerManager::stopEnlargeWidget()
{
    unschedule("enlarge_widget");
    _enlargeSprite->setVisible(false);
}

void LayerManager::disruptEnlargeWidget()
{
    // Mouse down while enlarging
    if (_enlarging)
    {
        stopEnlargeWidget();
        _enlarging = true;
    }
}

void LayerManager::addNodeToObjectLayer(Node* widget)
{
    _objectLayer->addChild(widget, _objectLayer->getChildrenCount());
}

void LayerManager::addNodeToCollectionLayer(Node* widget)
{
    _collectionLayer->addChild(widget, _collectionLayer->getChildrenCount());
}

void LayerManager::addNodeToInteractiveLayer(Node* widget)
{
    _interactiveLayer->addChild(widget, _interactiveLayer->getChildrenCount());
}

void LayerManager::addNodeToNonInteractiveLayer(Node* widget)
{
    _nonInteractiveLayer->addChild(widget, _nonInteractiveLayer->getChildrenCount());
}

void LayerManager::removeWidgetFromObjectLayer(Widget* widget)
{
    _objectLayer->removeChild(widget);
}

void LayerManager::removeWidgetFromCollectionLayer(Widget* widget)
{
    _collectionLayer->removeChild(widget);
}

void LayerManager::removeWidgetFromInteractiveLayer(Widget* widget)
{
    _interactiveLayer->removeChild(widget);
}

void LayerManager::removeWidgetFromNonInteractiveLayer(Widget* widget)
{
    _nonInteractiveLayer->removeChild(widget);
}

void LayerManager::removeWidgetFromAllLayers(Widget* widget)
{
    removeWidgetFromObjectLayer(widget);
    removeWidgetFromCollectionLayer(widget);
    removeWidgetFromInteractiveLayer(widget);
    removeWidgetFromNonInteractiveLayer(widget);
}

bool LayerManager::isWidgetInLayer(Widget* widget)
{
    Node* parent = widget->getParent();
    return parent == _objectLayer || parent == _collectionLayer || parent == _interactiveLayer ||
           parent == _nonInteractiveLayer;
}

void LayerManager::moveWidgetToFront(Widget* widget) {
    moveWidgetToSpecificOrder(widget, widget->getParent()->getChildrenCount() - 1);
}

void LayerManager::moveWidgetToBack(Widget* widget) {
    moveWidgetToSpecificOrder(widget, 0);
}

void LayerManager::moveWidgetForward(Widget* widget) {
    moveWidgetToSpecificOrder(widget, widget->getLocalZOrder() + 1);
}

void LayerManager::moveWidgetBackward(Widget* widget) {
    moveWidgetToSpecificOrder(widget, widget->getLocalZOrder() - 1);
}

void LayerManager::moveWidgetToSpecificOrder(Widget* widget, int order) {
    // Change zorder of the widget
    Node* layer = widget->getParent();
    layer->sortAllChildren();  // Ensure children are sorted so the algo correct
    AXASSERT(layer != nullptr, "Widget has no parent layer");

    int count = layer->getChildrenCount();
    auto& children = layer->getChildren();
    
    int currentOrder = widget->getLocalZOrder();

    // make sure order is within bounds
    if (order < 0) order = 0;
    if (order >= count) order = count - 1;

    if (currentOrder == order) return;

    for (int i = 0; i < count; ++i)
    {
        if (children.at(i) == widget)
        {
            AXASSERT(i == currentOrder, "Widget order must equal its index when sorted");
            widget->setLocalZOrder(order);
            int offset = (order > i) ? 1 : -1;
            // Adjust the z-order of other widgets in the layer
            for (int j = i + offset; j >= 0 && j < count; j += offset)
            {
                auto child = children.at(j);
                if (child != widget)
                {
                    child->setLocalZOrder(child->getLocalZOrder() - offset);
                }
                // We only move offseted amount to shift necessary widget order
                if (order - currentOrder == j - i)
                    return;
            }

            break;
        }
    }

}

void LayerManager::moveWidgetsToFront(std::set<Widget*>& widgets) {
    moveWidgetsByOffsetPreparation(widgets, 0);
}

void LayerManager::moveWidgetsToBack(std::set<Widget*>& widgets) {
    moveWidgetsByOffsetPreparation(widgets, 2);
}

void LayerManager::moveWidgetsForward(std::set<Widget*>& widgets) {
    moveWidgetsByOffsetPreparation(widgets, 1);
}

void LayerManager::moveWidgetsBackward(std::set<Widget*>& widgets) {
    moveWidgetsByOffsetPreparation(widgets, 3);
}

void LayerManager::moveWidgetsByOffsetPreparation(std::set<Widget*>& widgets, int type) {
    // If widgets is in holder then the holder manage their order
    vector<Widget*> heldWidgets;
    for (auto& widget : widgets)
    {
        if (widget->isHeld())
            heldWidgets.push_back(widget);

    }
    // Reason for bug with children
    for (auto& widget : heldWidgets)
    {
        widgets.erase(widget);
    }
    sortAllLayersChildren();
    map<Node*, vector<Widget*>> needMovingWidgets;
    map<Node*, Vector<Node*>> layerChildren;

    for (auto& layer : _layerList)
    {
        layerChildren[layer] = layer->getChildren();
    }

    for (auto& [layer, children] : layerChildren)
    {
        needMovingWidgets[layer] = vector<Widget*>();
        for (auto& child : children)
        {
            if (widgets.find(static_cast<Widget*>(child)) != widgets.end())
            {
                needMovingWidgets[layer].push_back(static_cast<Widget*>(child));
            }
        }

        if (needMovingWidgets[layer].empty())
            continue;

        long long minOrder, maxOrder, offset = 0;
        minOrder = needMovingWidgets[layer].front()->getLocalZOrder();
        maxOrder = needMovingWidgets[layer].back()->getLocalZOrder();
        if (type == 0)  // move to front
        {
            offset = max(layer->getChildrenCount() - 1 - maxOrder, 1LL);
        }
        else if (type == 1)  // move forward
        {
            offset = 1;
        }
        else if (type == 2)  // move to back
        {
            offset = min(-minOrder, -1LL);
        }
        else if (type == 3)  // move backward
        {
            offset = -1;
        }
        moveWidgetsOnSpecificLayerByOffset(layerChildren[layer], needMovingWidgets[layer], offset);

    }


}

void LayerManager::moveWidgetsOnSpecificLayerByOffset(ax::Vector<Node*>& layerChildren,
                                                      std::vector<Widget*>& needMovingWidgets,
                                                      long long offset)
{
    int increment  = (offset > 0) ? 1 : -1;

    int bound = (offset > 0) ? min(layerChildren.size() - 1, needMovingWidgets.back()->getLocalZOrder() + offset)
                             : max(0LL, needMovingWidgets.front()->getLocalZOrder() + offset);

    int startIndex = (offset > 0) ? needMovingWidgets.front()->getLocalZOrder() : needMovingWidgets.back()->getLocalZOrder();

    int lowOrder = startIndex;
    int highOrder = bound + increment * (-needMovingWidgets.size() + 1);

    for (int i = startIndex; (offset > 0) ? (i <= bound) : (i >= bound); i += increment)
    {
        auto child = static_cast<Widget*>(layerChildren.at(i));
        AXASSERT(child != nullptr, "Layer child is null");
        if (std::find(needMovingWidgets.begin(), needMovingWidgets.end(), child) != needMovingWidgets.end())
        {
            child->setLocalZOrder(highOrder);
            highOrder += increment;
        }
        else
        {
            child->setLocalZOrder(lowOrder);
            lowOrder += increment;
        }
    }

    for (auto& child : layerChildren)
    {
        AXLOGD("Child {}: z order {}", child->getName(), child->getLocalZOrder());
    }
    AXLOGD("-------------WIDGETS LAYER AFTER MOVED---------");
}

void LayerManager::hideWidget(Widget* widget)
{
    widget->pause();
    widget->setVisible(false);
}

void LayerManager::showWidget(Widget* widget)
{
    widget->resume();
    widget->setVisible(true);
}

void LayerManager::deleteWidgets(set<Widget*>& widgets) {
    auto inputManager = InputManager::getInstance();
    AXASSERT(inputManager != nullptr, "InputManager instance is null");
    for (auto widget : widgets)
    {
        widget->removeFromParentAndCleanup(true);
    }
    // Dont use the clearSelectedWidgets() function here,
    // because it will clear the visuals of the widgets, which are already deleted
    if (inputManager->isWidgetSelected(inputManager->getSelectedAloneWidget()))
    {
        inputManager->setSelectedAloneWidget(nullptr);
    }
    if (inputManager->isWidgetSelected(inputManager->getHoveringWidget()))
    {
        inputManager->setHoveringWidget(nullptr);
    }
    widgets.clear();
}

void LayerManager::duplicateWidgets(set<Widget*>& widgets) {
    // Sort the widgets by their z-order to maintain the order when duplicating
    std::vector<Widget*> sortedWidgets(widgets.begin(), widgets.end());
    vector<Widget*> newWidgets;
    std::sort(sortedWidgets.begin(), sortedWidgets.end(),
              [](Widget* a, Widget* b) { return a->getLocalZOrder() < b->getLocalZOrder(); }
    );
    for (auto widget : widgets)
    {
        Widget* newWidget = widget->clone();
        newWidget->addToLayer();
        newWidgets.push_back(newWidget);
    }
    // The newly duplicated widgets should be selected
    // And the previous selection should be cleared
    InputManager::getInstance()->clearSelectedWidgets();
    for (auto newWidget : newWidgets)
    {
        InputManager::getInstance()->addSelectedWidget(newWidget);
    }
}

void LayerManager::sortAllLayersChildren() {
    for (auto& layer : _layerList)
    {
        layer->sortAllChildren();
    }
}

Holder* LayerManager::findHoveredOnHolder(Widget* widget)
{
    // Holder on non interactive layer
    auto nonInteractiveWidgets = _nonInteractiveLayer->getChildren();
    float mtvMaxLength            = 0;
    Holder* bestFitHolder      = nullptr;
    for (auto node : nonInteractiveWidgets)
    {
        Holder* holder = dynamic_cast<Holder*>(node);
        if (holder == nullptr || holder->getType() != "Holder" || holder == widget)
            continue;
        Vec2 mtv(0, 0);
        if (holder->isWidgetHoveringOn(widget, mtv))
        {
            float mtvLength = mtv.length();
            if (mtvLength > mtvMaxLength)
            {
                mtvMaxLength = mtvLength;
                bestFitHolder = holder;
            }
        }
    }
    return bestFitHolder;
}
