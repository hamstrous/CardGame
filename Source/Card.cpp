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
    _frontSprite = ax::Sprite::create(frontTexture);
    _backSprite  = ax::Sprite::create(backTexture);
    
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

void Card::flip(float duration) {
    _isFaceUp = !_isFaceUp;
    AXLOGD("flip");
    auto scaleDown = ScaleTo::create(duration / 2, 0.0f, 1.0f);
    auto scaleUp   = ScaleTo::create(duration / 2, 1.0f, 1.0f);
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
    auto sequence = Sequence::create(scaleDown, swapSprites, scaleUp, nullptr);
    this->runAction(sequence);
}

void Card::setFaceUp(bool faceUp) {
    if (_isFaceUp != faceUp)
    {
        flip(0.0f); // Instant flip
    }
}

bool Card::containsPoint(const ax::Vec2& worldPoint) const
{
    Vec2 localPoint = this->convertToNodeSpace(worldPoint);
    Rect bbox       = Rect(-this->getContentSize().width/2,
                           -this->getContentSize().height/2,
                            this->getContentSize().width,this->getContentSize().height);
    if (bbox.containsPoint(localPoint))
        return true;
    return false;
}

void Card::enableDragging(bool enable) {
    _isDraggable = enable;
}

void Card::setHighlight(bool highlight) {

    if (highlight && !_highlightNode)
    {
        _highlightNode = ax::DrawNode::create();
        Vec2 rect[4];
        float w = this->getContentSize().width;
        float h = this->getContentSize().height;
        rect[0] = Vec2(-w/2, -h/2);
        rect[1] = Vec2(w/2, -h/2);
        rect[2] = Vec2(w/2, h/2);
        rect[3] = Vec2(-w/2, h/2);
        _highlightNode->drawPoly(rect, 4, true, Color4F::YELLOW);
        this->addChild(_highlightNode, 100);
        this->setScale(_originalScale * 1.05f);
    }
    else if (!highlight && _highlightNode)
    {
        this->removeChild(_highlightNode);
        _highlightNode = nullptr;
        this->setScale(_originalScale);
    }
}
