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

void LayerManager::moveWidgetToSpecificOrder(Widget* widget, int order) {
    // Change zorder of the widget
    Node* layer = widget->getParent();
    AXASSERT(layer != nullptr, "Widget has no parent layer");

}
