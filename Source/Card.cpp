#include "Card.h"
using namespace ax;

const ax::Vec2 Card::CARD_SIZE{82, 127.5f};
const int Card::MOVE_TO_HOLDER_TAG = 10;

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

Card* Card::clone() const
{
    Card* newCard = new (std::nothrow) Card();
    newCard->autorelease();
    if (newCard)
    {
        // Copy front and back textures
        newCard->_frontSprite = Sprite::createWithTexture(_frontSprite->getTexture());
        newCard->_backSprite  = Sprite::createWithTexture(_backSprite->getTexture());

        newCard->_frontSprite->setContentSize(CARD_SIZE);
        newCard->_backSprite->setContentSize(CARD_SIZE);
        newCard->addChild(newCard->_backSprite);
        newCard->addChild(newCard->_frontSprite);

        newCard->_objectSize = CARD_SIZE;

        // Set visibility based on current card state
        if (_isFaceUp)
        {
            newCard->_frontSprite->setVisible(true);
            newCard->_backSprite->setVisible(false);
        }
        else
        {
            newCard->_frontSprite->setVisible(false);
            newCard->_backSprite->setVisible(true);
        }
        newCard->_isFaceUp = _isFaceUp;
        newCard->setContentSize(CARD_SIZE);

        //Do not set position
        return newCard;
    }
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
        flip();
    }
}

void Card::zoomToCenter(const ax::Vec2& screenCenter, float zoomScale, float duration)
{
    // Store original position and scale
    setOriginalPosition(getPosition());
    setOriginalZOrder();
    this->setLocalZOrder(10000); // Bring to front
    // Create zoom and move actions
    auto moveTo    = MoveTo::create(duration, screenCenter);
    auto scaleTo   = ScaleTo::create(duration, zoomScale);
    auto easeMove  = EaseOut::create(moveTo, 2.0f);
    auto easeScale = EaseOut::create(scaleTo, 2.0f);

    // Run actions in parallel
    auto spawn = Spawn::create(easeMove, easeScale, nullptr);
    this->runAction(spawn);
}

void Card::unzoom(float duration)
{
    // Return to original position and scale
    this->setLocalZOrder(getOriginalZOrder());

    auto moveTo    = MoveTo::create(duration, getOriginalPosition());
    auto scaleTo   = ScaleTo::create(duration, 1.0f);
    auto easeMove  = EaseOut::create(moveTo, 2.0f);
    auto easeScale = EaseOut::create(scaleTo, 2.0f);

    // Run actions in parallel
    auto spawn = Spawn::create(easeMove, easeScale, nullptr);
    this->runAction(spawn);
}
