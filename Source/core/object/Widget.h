#pragma once

#include "axmol.h"

#include <string>
#include <map>

#include "utils/helper.h"
#include "utils/Timer.hpp"

using namespace ax; 

class Widget : public Node
{

public:

    // Factory method
    static Widget* create();
    bool init();
    void update(float delta) override;

    Widget() = default;
    ~Widget() = default;

    virtual bool onMouseDown(ax::Event* event);
    // for testing
    void setSizeAndDraw(const ax::Size& size);

    virtual void drawProperties();

    int getCount() const { return _count; }

    virtual std::string getId() const { return getType() + "_" + std::to_string(_id); }

    virtual std::string getType() const { return "Widget"; }

protected:
    static int _count;
    int _id;

    EventListenerMouse* _mouseListener;
    DrawNode* _drawNode;

};
