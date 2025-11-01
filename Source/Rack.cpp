#include "Rack.h"
#include <limits>
#include <algorithm>
using namespace ax;

Rack* Rack::create(const std::string& texture)
{
    Rack* rack = new (std::nothrow) Rack();
    if (rack && rack->init(texture))
    {
        rack->autorelease();
        return rack;
    }
    AX_SAFE_DELETE(rack);
    return nullptr;
}

bool Rack::init(const std::string& texture)
{
    if (!Node::init())
        return false;

    _objectSize   = TABLE_SIZE;
    _isDraggable  = false;
    _holderOffset = TABLE_OFFSET;

    _tableSprite = Sprite::create(texture);
    if (!_tableSprite)
    {
        AXLOG("Failed to load rack texture: %s", texture.c_str());
        return false;
    }

    _tableSprite->setContentSize(TABLE_SIZE);
    this->addChild(_tableSprite);
    setContentSize(TABLE_SIZE);
    return true;
}
