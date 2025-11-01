#pragma once

#include "axmol.h"
#include "Card.h"
#include "Holder.h"

class Table : public Holder
{
public:
    static Table* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    static const ax::Vec2 TABLE_SIZE;
    static const ax::Vec2 TABLE_OFFSET;

private:
    ax::Sprite* _tableSprite = nullptr;
};
