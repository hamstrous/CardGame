#pragma once

#include "axmol.h"
#include "Card.h"
#include "Holder.h"

class Rack : public Holder
{
public:
    static Rack* create(const std::string& texture);

    virtual bool init(const std::string& texture);
    Rack* clone() const;

    // Set rack configuration from config file
    void setConfig(int id, float posX, float posY, float sizeX, float sizeY, float rotation);

    static const ax::Vec2 DECK_SIZE;
    static const ax::Vec2 DECK_OFFSET;

private:
    ax::Sprite* _rackSprite = nullptr;
};
