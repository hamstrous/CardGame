#pragma once

#include "axmol.h"

#include "core/object/Widget.h"

#include "ImGui/ImGuiPresenter.h"

USING_NS_AX_EXT;
using namespace ax;
using namespace std;

class SelectManager : public ax::Node
{
public:
    SelectManager();

    static SelectManager* getInstance();
    static void destroyInstance();

    bool onMouseDown(Event* event);
    bool onMouseMove(Event* event);
    bool onMouseUp(Event* event);

    void addSelectedWidget(Widget* widget) {
        _selectedWidgets.insert(widget);
    }

    set<Widget*>& getSelectedWidgets() { return _selectedWidgets; }

private:
    static SelectManager* _instance;

    EventListenerMouse* _mouseListener;

    DrawNode* _drawNode;  // For drawing rubber band selection
    set<Widget*> _selectedWidgets;  // Store selected widgets

    Vec2 _startPoint;  // Starting point of the selection rectangle
};
