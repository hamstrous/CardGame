#pragma once

#include "axmol.h"

#include <string>
#include <map>

#include "utils/helper.h"
#include "utils/Timer.hpp"

#include "core/object/Widget.h"

using namespace ax; 

class Piece : public Widget
{
public:

    // Factory method
    static Piece* create(std::string type = "Piece");
    bool init(std::string type);
    void update(float delta) override;

    //void setUp() override;

    Piece() = default;
    ~Piece() = default;


    void drawProperties() override;

    int getCount() const { return _count; }

    std::string getType() const override { return _type; } 

protected:

    static int _count;

    // General properties
    bool _hide = false;
    bool _scalable = true;
    bool _rotatable = true;
        
    // Edit mode properties
    bool _hideInEditMode = false;


    // Play mode properties
    bool _allowClick = true;
    bool _allowDrag  = true;
    int _enlargeFactor = 0; // pop up a large view when hover; max = 4


};
