#pragma once

#include "axmol.h"

#include "ZoneData.h"
#include "Card.h"

#include "utils/helper.h"
#include "utils/Timer.hpp"


class Zone : public ax::Node
{
public:
    static Zone* create(ZoneData* property);
    bool init(ZoneData* property);

    bool onMouseDown(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    void shuffle();
    void sendCardTo(Zone* targetZone, Card* card);
    void sortCards();

    ~Zone() override;

protected:
    ax::Vector<Card*> _cardList;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;

};
