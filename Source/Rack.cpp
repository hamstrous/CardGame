#include "Rack.h"
#include <limits>
#include <algorithm>
using namespace ax;

const ax::Vec2 Rack::DECK_SIZE{650, 170};
const ax::Vec2 Rack::DECK_OFFSET{250, 0};

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

    _objectSize   = DECK_SIZE;
    _isDraggable  = false;
    _holderOffset = DECK_OFFSET;

    _rackSprite = Sprite::create(texture);

    _maxCardSpacing = Card::CARD_SIZE.x;
    _cardSpacing    = Card::CARD_SIZE.x;
    if (!_rackSprite)
    {
        AXLOG("Failed to load rack texture: %s", texture.c_str());
        return false;
    }

    _rackSprite->setContentSize(DECK_SIZE);
    this->addChild(_rackSprite);
    setContentSize(DECK_SIZE);
    return true;
}
