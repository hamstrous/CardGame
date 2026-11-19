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

protected:
    static int _count;
    int _id;
    std::string _type = "Holder";

    float _roundedCorner = 0;

};
