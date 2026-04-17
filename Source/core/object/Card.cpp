#include "Card.h"
#include "Zone.h"
#include "core/event/EventCard.h"

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
    _property = property;

    this->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
    this->setTag(Card::CARD_TAG);

    _frontSprite = ax::Sprite::create(property->frontImagePath);
    _backSprite  = ax::Sprite::create(property->backImagePath);
    if (!_frontSprite || !_backSprite)
    {
        return false;
    }

    auto cardSize = _frontSprite->getContentSize();
    this->setContentSize(cardSize);

    this->addChild(_frontSprite);
    this->addChild(_backSprite);

    _frontSprite->setPosition(cardSize/2);
    _backSprite->setPosition(cardSize/2);

    _backSprite->setVisible(false);

    // init for event
    _mouseListener                = ax::EventListenerMouse::create();
    _mouseListener->onMouseDown   = AX_CALLBACK_1(Card::onMouseDown, this);
    _mouseListener->onMouseMove   = AX_CALLBACK_1(Card::onMouseMove, this);
    _mouseListener->onMouseUp     = AX_CALLBACK_1(Card::onMouseUp, this);
    _mouseListener->setSwallowMouse(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    //_keyboardListener                = ax::EventListenerKeyboard::create();
    //_keyboardListener->onKeyPressed  = AX_CALLBACK_2(MenuScene::onKeyPressed, this);
    //_keyboardListener->onKeyReleased = AX_CALLBACK_2(MenuScene::onKeyReleased, this);
    //_eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    scheduleUpdate();
    return true;
}

void Card::update(float delta) {}

bool Card::onMouseDown(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());
    if (isWorldPositionInNode(this, mousePos))  // containPoint(this,mousePos))
    {
        moveNodeToFront(this);
        this->setGlobalZOrder(100); 
        _clicktimer.reset();
        //_dragOffset = mousePos - getNodePositionInWorldSpace(this);
        if (this->getNumberOfRunningActionsByTag(Zone::CARD_TRANSFORM_TO_ZONE_ACTION_TAG) > 0)
        {
            // When cards are moving, clicking on it will consider as stopping it for dragging
            this->stopActionByTag(Zone::CARD_TRANSFORM_TO_ZONE_ACTION_TAG);
            _isDragging = true;
        }
        else
        {

            _clicktimer.start();
        }
        _dragOffset = this->getParent()->convertToNodeSpace(mousePos) - this->getPosition();
        return true; // Event swallowed
    }

    return false;  // Propagate to other listeners
}

bool Card::onMouseMove(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());
    bool ret          = false;
    // Drag logic
    if(!_isDraggable) return false;
    if (_clicktimer.count() > 0)
    {
        _clicktimer.reset();
        _isDragging = true;
        ret         = true;
        this->stopActionByTag(Zone::CARD_TRANSFORM_TO_ZONE_ACTION_TAG);  
    }

    if (_isDragging)
    {
        setPosition(this->getParent()->convertToNodeSpace(mousePos) - _dragOffset);
        ret = true;
    }
    return ret;
}

bool Card::onMouseUp(ax::Event* event)
{
    ax::EventMouse* e = static_cast<ax::EventMouse*>(event);
    auto mousePos     = ax::Vec2(e->getCursorX(), e->getCursorY());
    bool ret          = false;
    
    if (_clicktimer.count() <= 200 && _clicktimer.count() > 0)
    {
        _dragOffset = ax::Vec2::ZERO;
        flip();
        ret = true;  // Event swallowed
    }
    else if (_isDragging)
    {
        EventCard* event = new EventCard(this, mousePos);
        _eventDispatcher->dispatchEvent(event);
    }
    this->setGlobalZOrder(0);
    _isDragging = false;
    _clicktimer.reset();

    return ret;
}

void Card::setContentSize(const ax::Size& contentSize) {
    Node::setContentSize(contentSize);
    if (_frontSprite)
        _frontSprite->setContentSize(contentSize);
    if (_backSprite)
        _backSprite->setContentSize(contentSize);
    _frontSprite->setPosition(contentSize / 2);
    _backSprite->setPosition(contentSize / 2);
}

void Card::setGlobalZOrder(int z) {
    Node::setGlobalZOrder(z);
    _frontSprite->setGlobalZOrder(z);
    _backSprite->setGlobalZOrder(z);
}

void Card::flip(float duration) {
    auto runningFlipAction = this->getActionByTag(FLIP_ACTION_TAG);
    if (runningFlipAction)
    {
        return;
    }

    _isFaceUp        = !_isFaceUp;
    auto scaleDown   = ax::ScaleTo::create(duration / 2, 0.0f, 1.0f);
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

    EventCard* event = new EventCard(this, true);

    _eventDispatcher->dispatchEvent(event);
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

void Card::setCurrentZone(Zone* zone) {
    _currentZone = zone;
}

void Card::moveToZone(Zone* targetZone, float duration) {
    targetZone->moveCardToThisZone(this, duration);
}

Card::~Card()
{
    AX_SAFE_DELETE(_property);

    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
}

void Card::lockInput() {
    _mouseListener->setEnabled(false);
}

void Card::unlockInput() {
    _mouseListener->setEnabled(true);
}
