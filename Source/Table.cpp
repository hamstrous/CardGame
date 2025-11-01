#include "Table.h"
#include <limits>
#include <algorithm>
using namespace ax;

const ax::Vec2 Table::TABLE_SIZE{650, 170};
const ax::Vec2 Table::TABLE_OFFSET{250, 0};

Table* Table::create(const std::string& texture)
{
    Table* table = new (std::nothrow) Table();
    if (table && table->init(texture))
    {
        table->autorelease();
        return table;
    }
    AX_SAFE_DELETE(table);
    return nullptr;
}

bool Table::init(const std::string& texture)
{
    if (!Node::init())
        return false;

    _objectSize   = TABLE_SIZE;
    _isDraggable  = false;
    _holderOffset = TABLE_SIZE;

    _tableSprite = Sprite::create(texture);
    if (!_tableSprite)
    {
        AXLOG("Failed to load Table texture: %s", texture.c_str());
        return false;
    }

    _tableSprite->setContentSize(TABLE_SIZE);
    this->addChild(_tableSprite);
    setContentSize(TABLE_SIZE);
    return true;
}
