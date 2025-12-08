#pragma once

#include "axmol.h"
#include "Card.h"
#include "Holder.h"

class Rack : public Holder
{
public:
    static Rack* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    static const ax::Vec2 DECK_SIZE;
    static const ax::Vec2 DECK_OFFSET;

private:
    ax::Sprite* _rackSprite = nullptr;
};
