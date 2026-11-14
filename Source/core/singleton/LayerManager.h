#pragma once

#include "axmol.h"

class Widget;

// This class organize the order each widget appear on screen
// object -> collection -> interactive -> non interactive
// Ensure the order of each widget is from 0 to count, with no gap or duplicate

class LayerManager : public ax::Node
{
public:
    LayerManager();

    static LayerManager* getInstance();
    static void destroyInstance();

    void addWidgetToObjectLayer(Widget* widget);
    void addWidgetToCollectionLayer(Widget* widget);
    void addWidgetToInteractiveLayer(Widget* widget);
    void addWidgetToNonInteractiveLayer(Widget* widget);

    void removeWidgetFromObjectLayer(Widget* widget);
    void removeWidgetFromCollectionLayer(Widget* widget);
    void removeWidgetFromInteractiveLayer(Widget* widget);
    void removeWidgetFromNonInteractiveLayer(Widget* widget);

    void removeWidgetFromAllLayers(Widget* widget);
    
    bool isWidgetInLayer(Widget* widget);

    void moveWidgetToFront(Widget* widget);
    void moveWidgetToBack(Widget* widget);
    void moveWidgetForward(Widget* widget);
    void moveWidgetBackward(Widget* widget);
    void moveWidgetToSpecificOrder(Widget* widget, int order);

    void hideWidget(Widget* widget);
    void showWidget(Widget* widget);

    void deleteWidgets(std::set<Widget*>& widgets);
    void duplicateWidgets(std::set<Widget*>& widgets);

private:
    static LayerManager* _instance;

    Node* _objectLayer         = nullptr;
    Node* _collectionLayer     = nullptr;
    Node* _interactiveLayer    = nullptr;
    Node* _nonInteractiveLayer = nullptr;
};
