#include "Card.h"

Card* Card::create(const CardProperty* property)
{
    Card* card = new (std::nothrow) Card();
    if (card && card->init(property))
    {
        card->autorelease();
        return card;
    }
    AX_SAFE_DELETE(card);
    return nullptr;
}

bool Card::init(const CardProperty* property)
{
    if (!Node::init())
    {
        return false;
    }

    // TODO: Initialize from property

    return true;
}

void Card::setContentSize(const ax::Size& contentSize) {
    Node::setContentSize(contentSize);
    if (_frontSprite)
        _frontSprite->setContentSize(contentSize);
    if (_backSprite)
        _backSprite->setContentSize(contentSize);
}

Card::~Card() {
    AX_SAFE_DELETE(_property);

    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
}
