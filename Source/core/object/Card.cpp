#include "Card.h"

Card* Card::create(CardData* property)
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

bool Card::init(CardData* property)
{
    if (!Node::init())
    {
        return false;
    }

    this->setAnchorPoint(ax::Vec2(0.5f, 0.5f)); 

    _property = property;

    _frontSprite = ax::Sprite::create(property->frontImagePath);
    _backSprite  = ax::Sprite::create(property->backImagePath);
    if (!_frontSprite || !_backSprite)
    {
        return false;
    }

    this->addChild(_frontSprite);
    this->addChild(_backSprite);
    _frontSprite->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
    _backSprite->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
    _frontSprite->setPosition(0,0);
    _backSprite->setPosition(0,0);

    _backSprite->setVisible(false);

    this->setContentSize(_frontSprite->getContentSize());

    auto drawNode = ax::DrawNode::create();
    auto cardSize = this->getContentSize();
    // Draw rectangle showing the bounding box of the Card itself
    drawNode->drawRect(ax::Vec2(0, 0), ax::Vec2(cardSize.width, cardSize.height), ax::Color4F::RED);

    // Display anchor point of the card and positions of its sprites
    drawNode->drawDot(ax::Vec2(cardSize.width * getAnchorPoint().x, cardSize.height * getAnchorPoint().y), 5, ax::Color4F::RED);
    drawNode->drawDot(_frontSprite->getPosition(), 5, ax::Color4F::GREEN);
    drawNode->drawDot(_backSprite->getPosition(), 5, ax::Color4F::BLUE);
    this->addChild(drawNode, 100);

    // init for event
    _mouseListener                = ax::EventListenerMouse::create();
    _mouseListener->onMouseDown   = AX_CALLBACK_1(Card::onMouseDown, this);
    _mouseListener->setSwallowMouse(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    //_keyboardListener                = ax::EventListenerKeyboard::create();
    //_keyboardListener->onKeyPressed  = AX_CALLBACK_2(MenuScene::onKeyPressed, this);
    //_keyboardListener->onKeyReleased = AX_CALLBACK_2(MenuScene::onKeyReleased, this);
    //_eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    return true;
}

bool Card::onMouseDown(ax::Event* event)
{
    // Check if card is clicked
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());

    if (this->getBoundingBox().containsPoint(mousePos))//containPoint(this,mousePos))
    {
        flip();
        return true; // Event swallowed
    }

    return false;  // Propagate to other listeners
}

void Card::setContentSize(const ax::Size& contentSize) {
    Node::setContentSize(contentSize);
    if (_frontSprite)
        _frontSprite->setContentSize(contentSize);
    if (_backSprite)
        _backSprite->setContentSize(contentSize);
}

void Card::flip(float duration) {
    this->stopActionByTag(FLIP_ACTION_TAG); 

    _isFaceUp        = !_isFaceUp;
    auto scaleDown   = ax::ScaleTo::create(duration / 2, 0.0f, 0.0f);
    auto scaleUp     = ax::ScaleTo::create(duration / 2, 1.0f, 1.0f);
    auto swapSprites = ax::CallFunc::create([this]() {
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

    auto sequence    = ax::Sequence::create(scaleDown, swapSprites, scaleUp, nullptr);
    sequence->setTag(FLIP_ACTION_TAG);
    this->runAction(sequence);
}

void Card::reveal() {
    if(_isFaceUp) return;
    flip();
}

void Card::hide() {
    if (!_isFaceUp) return;
    flip();
}   

void Card::setDraggable(bool draggable) {
    _isDraggable = draggable;
}

bool Card::getDraggable()
{
    return _property->isDraggable;
}

void Card::setFaceUp(bool faceUp) {
    _property->isFaceUp = faceUp;
}

bool Card::getFaceUp()
{
    return _property->isFaceUp;
}

Card::~Card() {
    AX_SAFE_DELETE(_property);

    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
}
