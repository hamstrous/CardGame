#pragma once

#include "axmol.h"
#include "Card.h"
#include "Holder.h"

class Rack : public Holder
{
public:
    static Rack* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    inline static const ax::Vec2 TABLE_SIZE{650, 170};
    inline static const ax::Vec2 TABLE_OFFSET{250, 0};

private:
    ax::Sprite* _tableSprite = nullptr;
};
