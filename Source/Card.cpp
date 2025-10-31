#include "Card.h"
using namespace ax;

Card* Card::create(const std::string& frontTexture, const std::string& backTexture)
{
    Card* card = new (std::nothrow) Card();
    if (card && card->init(frontTexture, backTexture))
    {
        card->autorelease();
        return card;
    }
    AX_SAFE_DELETE(card);
    return nullptr;
}

bool Card::init(const std::string& frontTexture, const std::string& backTexture)
{
    if (!Node::init())
        return false;

    _objectSize = CARD_SIZE;

    _frontSprite = Sprite::create(frontTexture);
    _backSprite  = Sprite::create(backTexture);

    if (!_frontSprite || !_backSprite)
        return false;

    _frontSprite->setContentSize(CARD_SIZE);
    _backSprite->setContentSize(CARD_SIZE);
    this->addChild(_backSprite);
    this->addChild(_frontSprite);
    _frontSprite->setVisible(false);
    _backSprite->setVisible(true);
    _isFaceUp = false;
    setContentSize(CARD_SIZE);
    return true;
}

void Card::flip(float duration)
{
    _isFaceUp        = !_isFaceUp;
    auto scaleDown   = ScaleTo::create(duration / 2, 0.0f, 1.0f);
    auto scaleUp     = ScaleTo::create(duration / 2, 1.0f, 1.0f);
    auto swapSprites = CallFunc::create([this]() {
        if (_isFaceUp)
        {
            _frontSprite->setVisible(true);
            _backSprite->setVisible(false);
        }
        else
        {
            _frontSprite->setVisible(false);
            _backSprite->setVisible(true);
        }
    });
    auto sequence    = Sequence::create(scaleDown, swapSprites, scaleUp, nullptr);
    this->runAction(sequence);
}

void Card::setFaceUp(bool faceUp)
{
    if (_isFaceUp != faceUp)
    {
        flip(0.0f);
    }
}
