#pragma once

#include "axmol.h"

#include "core/object/Widget.h"
#include "core/const/GameConstants.h"

#include "core/event/EventMode.h"
#include "core/event/EventListenerMode.h"

#include "ImGui/ImGuiPresenter.h"

USING_NS_AX_EXT;
using namespace ax;
using namespace std;
using namespace constant;

// This class organize the order each widget appear on screen
// object -> collection -> interactive -> non interactive

class LayerManager : public ax::Node
{
public:
    LayerManager();

    static LayerManager* getInstance();
    static void destroyInstance();

    void addWidgetToObjectLayer(Widget* widget) { _objectLayer->addChild(widget); }
    void addWidgetToCollectionLayer(Widget* widget) { _collectionLayer->addChild(widget); }
    void addWidgetToInteractiveLayer(Widget* widget) { _interactiveLayer->addChild(widget); }
    void addWidgetToNonInteractiveLayer(Widget* widget) { _nonInteractiveLayer->addChild(widget); }

    void removeWidgetFromObjectLayer(Widget* widget) { _objectLayer->removeChild(widget); }
    void removeWidgetFromCollectionLayer(Widget* widget) { _collectionLayer->removeChild(widget); }
    void removeWidgetFromInteractiveLayer(Widget* widget) { _interactiveLayer->removeChild(widget); }
    void removeWidgetFromNonInteractiveLayer(Widget* widget) { _nonInteractiveLayer->removeChild(widget); }
    void removeWidgetFromAllLayers(Widget* widget);
    

    void moveWidgetToFront(Widget* widget);
    void moveWidgetToBack(Widget* widget);
    void moveWidgetForward(Widget* widget);
    void moveWidgetBackward(Widget* widget);
    void moveWidgetToSpecificOrder(Widget* widget, int order);

private:
    static LayerManager* _instance;

    Node* _objectLayer         = nullptr;
    Node* _collectionLayer     = nullptr;
    Node* _interactiveLayer    = nullptr;
    Node* _nonInteractiveLayer = nullptr;
};
