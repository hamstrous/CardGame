#pragma once

#include "axmol.h"

#include "core/object/Widget.h"

class Holder : public Widget
{

public:

    // Factory method
    static Holder* create();
    void addToLayer();
    bool init();
    Holder* clone() const override;

    void drawGui() override;

    void update(float delta) override;

    // Override
    void setUpMainVisual() override;

    Holder() = default;
    ~Holder() = default;    

    //InspectorData createInspectorData() override;

    std::string getType() const override { return "Holder"; }

    bool isWidgetHoveringOn(Widget* widget, ax::Vec2& mtv);

    void otherHoveringOnAbove(Widget* widget) override;

protected:
    static int _count;
    int _id;

    float _roundedCorner = 0.5;
    bool _alignChilren   = true;
    int _dropLimit       = -1;
    bool _showLimit      = false;
    bool _dropShadow     = true;
    bool _preventPile    = false;
    std::vector<std::string> _dropTarget;
};
