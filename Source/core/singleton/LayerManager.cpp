#include "LayerManager.h"

#include "InputManager.h"
#include "ImGui/ImGuiPresenter.h"

#include "core/object/Widget.h"
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
    _objectLayer = Node::create();
    _collectionLayer = Node::create();
    _interactiveLayer = Node::create();
    _nonInteractiveLayer = Node::create();

    this->addChild(_objectLayer, 3);
    this->addChild(_collectionLayer, 2);
    this->addChild(_interactiveLayer, 1);
    this->addChild(_nonInteractiveLayer, 0);
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

void LayerManager::addWidgetToObjectLayer(Widget* widget)
{
    _objectLayer->addChild(widget, _objectLayer->getChildrenCount());
}

void LayerManager::addWidgetToCollectionLayer(Widget* widget)
{
    _collectionLayer->addChild(widget, _collectionLayer->getChildrenCount());
}

void LayerManager::addWidgetToInteractiveLayer(Widget* widget)
{
    _interactiveLayer->addChild(widget, _interactiveLayer->getChildrenCount());
}

void LayerManager::addWidgetToNonInteractiveLayer(Widget* widget)
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
            }

            break;
        }
    }

}

void LayerManager::hideWidget(Widget* widget) {
    widget->pause();
    widget->setVisible(false);
}

void LayerManager::showWidget(Widget* widget)
{
    widget->resume();
    widget->setVisible(true);
}

void LayerManager::deleteWidgets(set<Widget*>& widgets) {
    for (auto widget : widgets)
    {
        widget->removeFromParentAndCleanup(true);
    }
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
