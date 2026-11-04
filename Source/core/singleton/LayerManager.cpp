#include "LayerManager.h"

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

void LayerManager::removeWidgetFromAllLayers(Widget* widget)
{
    removeWidgetFromObjectLayer(widget);
    removeWidgetFromCollectionLayer(widget);
    removeWidgetFromInteractiveLayer(widget);
    removeWidgetFromNonInteractiveLayer(widget);
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
