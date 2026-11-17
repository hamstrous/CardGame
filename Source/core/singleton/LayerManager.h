#pragma once

#include "axmol.h"

class Widget;
class EventListenerMode;
class EventMode;
// This class organize the order each widget appear on screen
// object -> collection -> interactive -> non interactive
// Ensure the order of each widget is from 0 to count, with no gap or duplicate

class LayerManager : public ax::Node
{
public:
    LayerManager();

    static LayerManager* getInstance();
    static void destroyInstance();

    // Event
    void onWidgetHover(ax::EventCustom* event);
    void onWidgetStopHover(ax::EventCustom* event);

    bool onModeChanged(EventMode* event);

    bool onMouseDown(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    void enlargeWidget(Widget* widget);
    void stopEnlargeWidget();
    void disruptEnlargeWidget();


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

    void moveWidgetsToFront(std::set<Widget*>& widgets);
    void moveWidgetsToBack(std::set<Widget*>& widgets);
    void moveWidgetsForward(std::set<Widget*>& widgets);
    void moveWidgetsBackward(std::set<Widget*>& widgets);

    void moveWidgetsByOffsetPreparation(std::set<Widget*>& widgets, int type); //0 : front, 1: forward, 2: back, 3: backward
    void moveWidgetsOnSpecificLayerByOffset(ax::Vector<Node*>& layerChildren, std::vector<Widget*>& needMovingWidgets, long long offset);

    void hideWidget(Widget* widget);
    void showWidget(Widget* widget);

    void deleteWidgets(std::set<Widget*>& widgets);
    void duplicateWidgets(std::set<Widget*>& widgets);

    void sortAllLayersChildren();

private:
    static LayerManager* _instance;

    ax::Node* _objectLayer         = nullptr;
    ax::Node* _collectionLayer     = nullptr;
    ax::Node* _interactiveLayer    = nullptr;
    ax::Node* _nonInteractiveLayer = nullptr;

    bool _enlarging            = false;
    ax::Sprite* _enlargeSprite = nullptr;
    Widget* _hoverWidget       = nullptr;

    std::vector<ax::Node*> _layerList;

    // Event
    ax::EventListenerCustom* _customListener = nullptr;
    EventListenerMode* _modeListener;
    ax::EventListenerMouse* _mouseListener = nullptr;
};
