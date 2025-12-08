#include "Deck.h"
#include <limits>
#include <algorithm>
using namespace ax;

// Define DECK_SIZE in cpp file to avoid inline static initialization issues
const ax::Vec2 Deck::DECK_OFFSET{30, 30};
const ax::Vec2 Deck::DECK_SIZE = Card::CARD_SIZE + Deck::DECK_OFFSET;

Deck* Deck::create()
{
    Deck* deck = new (std::nothrow) Deck();
    if (deck && deck->init())
    {
        deck->autorelease();
        return deck;
    }
    AX_SAFE_DELETE(deck);
    return nullptr;
}

bool Deck::init(const ax::Color4F& color)
{
    if (!Node::init())
        return false;

    _objectSize   = DECK_SIZE;
    _isDraggable  = false;
    _holderOffset = DECK_OFFSET;

    _deckDrawNode = DrawNode::create();
    if (!_deckDrawNode)
    {
        AXLOG("Failed to create DrawNode for Deck");
        return false;
    }

    // Draw a rectangle with the specified color
    // Position corners relative to center (since node position is center)
    Vec2 bottomLeft(-DECK_SIZE.x / 2, -DECK_SIZE.y / 2);
    Vec2 topRight(DECK_SIZE.x / 2, DECK_SIZE.y / 2);

    _deckDrawNode->drawSolidRect(bottomLeft, topRight, color);

    this->addChild(_deckDrawNode);
    setContentSize(DECK_SIZE);

    // Create buttons for deal amount control
    _incrementButton = ax::ui::Button::create("ui/increase.png");
    _decrementButton = ax::ui::Button::create("ui/decrease.png");
    _resetButton     = ax::ui::Button::create("ui/reset.png");

    // Button size: small buttons on the left side of the deck
    ax::Vec2 buttonSize = ax::Vec2(DECK_SIZE.x * 0.25f, DECK_SIZE.x * 0.25f);
    _incrementButton->setScale9Enabled(true);
    _incrementButton->setCapInsets(ax::Rect(0, 0, 16, 16));
    _decrementButton->setScale9Enabled(true);
    _decrementButton->setCapInsets(ax::Rect(0, 0, 16, 16));
    _resetButton->setScale9Enabled(true);
    _resetButton->setCapInsets(ax::Rect(0, 0, 16, 16));

    _incrementButton->setContentSize(buttonSize);
    _decrementButton->setContentSize(buttonSize);
    _resetButton->setContentSize(buttonSize);

    // Position buttons on the left side of the deck, vertically stacked
    float buttonX = -DECK_SIZE.x / 2 - buttonSize.x / 2;
    _incrementButton->setPosition(ax::Vec2(buttonX, buttonSize.y));
    _decrementButton->setPosition(ax::Vec2(buttonX, 0));
    _resetButton->setPosition(ax::Vec2(buttonX, -buttonSize.y));

    this->addChild(_incrementButton);
    this->addChild(_decrementButton);
    this->addChild(_resetButton);

    // Add click handlers
    _incrementButton->addClickEventListener([this](auto sender) {
        _dealAmount += 1;
        if (_dealAmountLabel)
            _dealAmountLabel->setString(std::to_string(_dealAmount));
    });

    _decrementButton->addClickEventListener([this](auto sender) {
        if (_dealAmount > 0)
            _dealAmount -= 1;
        if (_dealAmountLabel)
            _dealAmountLabel->setString(std::to_string(_dealAmount));
    });

    _resetButton->addClickEventListener([this](auto sender) {
        _dealAmount = 1;
        if (_dealAmountLabel)
            _dealAmountLabel->setString(std::to_string(_dealAmount));
    });

    // // Add a mouse listener to stop event propagation when clicking on buttons
    // auto mouseListener         = ax::EventListenerMouse::create();
    // mouseListener->onMouseDown = [this](ax::Event* event) {
    //     auto e = static_cast<ax::EventMouse*>(event);
    //     ax::Vec2 mousePos(e->getCursorX(), e->getCursorY());
    //     ax::Vec2 localPos = this->convertToNodeSpace(mousePos);

    //     // Check if click is on any button
    //     if ((_incrementButton && _incrementButton->getBoundingBox().containsPoint(localPos)) ||
    //         (_decrementButton && _decrementButton->getBoundingBox().containsPoint(localPos)) ||
    //         (_resetButton && _resetButton->getBoundingBox().containsPoint(localPos)))
    //     {
    //         event->stopPropagation();  // Stop event from reaching MainScene
    //         return true;
    //     }
    //     return false;
    // };

    // _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // Create label at center top
    _dealAmountLabel = ax::Label::createWithSystemFont("1", "Arial", 20);
    _dealAmountLabel->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
    _dealAmountLabel->setPosition(ax::Vec2(0, DECK_SIZE.y / 2 + 15));
    _dealAmountLabel->setTextColor(ax::Color4B::BLACK);
    this->addChild(_dealAmountLabel);

    return true;
}

Deck* Deck::clone() const
{
    Deck* newDeck = new (std::nothrow) Deck();
    newDeck->autorelease();
    if (newDeck)
    {
        newDeck->_objectSize    = this->_objectSize;
        newDeck->_isDraggable   = this->_isDraggable;
        newDeck->_holderOffset  = this->_holderOffset;
        newDeck->_dealAmount    = this->_dealAmount;
        newDeck->_connectedHolders = this->_connectedHolders;

        // Clone DrawNode
        newDeck->_deckDrawNode = DrawNode::create();
        if (newDeck->_deckDrawNode && this->_deckDrawNode)
        {
            // Redraw the rectangle with the same color
            Vec2 bottomLeft(-DECK_SIZE.x / 2, -DECK_SIZE.y / 2);
            Vec2 topRight(DECK_SIZE.x / 2, DECK_SIZE.y / 2);

            // Assuming the original color is stored or can be retrieved
            // Here we just use a default color for demonstration
            ax::Color4F fillColor(0.7f, 0.7f, 0.7f, 0.9f);
            newDeck->_deckDrawNode->drawSolidRect(bottomLeft, topRight, fillColor);
            newDeck->addChild(newDeck->_deckDrawNode);
        }

        newDeck->setContentSize(this->getContentSize());

        // Clone buttons
        newDeck->_incrementButton = ax::ui::Button::create("ui/increase.png");
        newDeck->_decrementButton = ax::ui::Button::create("ui/decrease.png");
        newDeck->_resetButton     = ax::ui::Button::create("ui/reset.png");

        ax::Vec2 buttonSize = ax::Vec2(DECK_SIZE.x * 0.25f, DECK_SIZE.x * 0.25f);
        newDeck->_incrementButton->setScale9Enabled(true);
        newDeck->_incrementButton->setCapInsets(ax::Rect(0, 0, 16, 16));
        newDeck->_decrementButton->setScale9Enabled(true);
        newDeck->_decrementButton->setCapInsets(ax::Rect(0, 0, 16, 16));
        newDeck->_resetButton->setScale9Enabled(true);
        newDeck->_resetButton->setCapInsets(ax::Rect(0, 0, 16, 16));

        newDeck->_incrementButton->setContentSize(buttonSize);
        newDeck->_decrementButton->setContentSize(buttonSize);
        newDeck->_resetButton->setContentSize(buttonSize);

        float buttonX = -DECK_SIZE.x / 2 - buttonSize.x / 2;
        newDeck->_incrementButton->setPosition(ax::Vec2(buttonX, buttonSize.y));
        newDeck->_decrementButton->setPosition(ax::Vec2(buttonX, 0));
        newDeck->_resetButton->setPosition(ax::Vec2(buttonX, -buttonSize.y));

        newDeck->addChild(newDeck->_incrementButton);
        newDeck->addChild(newDeck->_decrementButton);
        newDeck->addChild(newDeck->_resetButton);

        newDeck->_incrementButton->addClickEventListener([newDeck](auto) {
            newDeck->_dealAmount += 1;
            if (newDeck->_dealAmountLabel)
                newDeck->_dealAmountLabel->setString(std::to_string(newDeck->_dealAmount));
        });

        newDeck->_decrementButton->addClickEventListener([newDeck](auto sender) {
            if (newDeck->_dealAmount > 0)
                newDeck->_dealAmount -= 1;
            if (newDeck->_dealAmountLabel)
                newDeck->_dealAmountLabel->setString(std::to_string(newDeck->_dealAmount));
        });

        _resetButton->addClickEventListener([newDeck](auto sender) {
            newDeck->_dealAmount = 1;
            if (newDeck->_dealAmountLabel)
                newDeck->_dealAmountLabel->setString(std::to_string(newDeck->_dealAmount));
        });

        // Clone label
        newDeck->_dealAmountLabel = ax::Label::createWithSystemFont(std::to_string(this->_dealAmount), "Arial", 20);
        newDeck->_dealAmountLabel->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
        newDeck->_dealAmountLabel->setPosition(ax::Vec2(0, DECK_SIZE.y / 2 + 15));
        newDeck->_dealAmountLabel->setTextColor(ax::Color4B::BLACK);
        newDeck->addChild(newDeck->_dealAmountLabel);
        return newDeck;
    }
    return nullptr;
}

void Deck::addCard(Card* card)
{
    _cards.push_back(card);
    for (int i = 0; i < _cards.size(); i++)
    {
        _cards[i]->setLocalZOrder(i);
    }
    card->stopAllActionsByTag(Card::MOVE_TO_HOLDER_TAG);
    auto moveTo   = MoveTo::create(0.1f, this->getPosition());
    auto easeOut  = EaseOut::create(moveTo, 1.0f);
    auto rotateTo = RotateTo::create(0.1f, this->getRotation());
    easeOut->setTag(Card::MOVE_TO_HOLDER_TAG);
    rotateTo->setTag(Card::MOVE_TO_HOLDER_TAG);
    card->runAction(rotateTo);
    card->runAction(easeOut);
}

void Deck::addCard(std::vector<Card*> cards)
{
    for (auto& card : cards)
    {
        addCard(card);
    }
}

void Deck::removeCard(Card* card)
{
    if (!card)
        return;
    auto it = std::find(_cards.begin(), _cards.end(), card);
    if (it != _cards.end())
    {
        _cards.erase(it);
    }
}

void Deck::removeCard(std::vector<Card*> card)
{
    for (auto& c : card)
    {
        removeCard(c);
    }
}

void Deck::shuffle()
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(_cards.begin(), _cards.end(), g);
    int zOrder   = 0;
    int index    = 0;
    float radius = 25.0f;
    for (auto& card : _cards)
    {
        float angle = AX_DEGREES_TO_RADIANS(360 / _cards.size() * index++);
        Vec2 offset = Vec2(cosf(angle), sinf(angle)) * radius;
        card->setLocalZOrder(zOrder++);
        auto moveAway   = MoveBy::create(0.3f, offset);
        auto moveToward = MoveBy::create(0.3f, -offset);
        auto seq        = Sequence::create(moveAway, moveToward, nullptr);
        card->runAction(seq);
    }
}

void Deck::deal()
{
    for (int i = 0; i < _dealAmount; ++i)
    {
        for (auto& holder : _connectedHolders)
        {
            if (_cards.empty())
            {
                return;  // No more cards to deal
            }
            Card* card = _cards.back();
            _cards.pop_back();
            holder->addCardToBack(card);
        }
    }
}

void Deck::dealSmoothly(float delayPerCard)
{
    float totalDelay = 0.0f;
    for (int i = 0; i < _dealAmount; ++i)
    {
        for (auto& holder : _connectedHolders)
        {
            if (_cards.empty())
            {
                return;  // No more cards to deal
            }
            Card* card = _cards.back();
            _cards.pop_back();
            auto delay      = DelayTime::create(totalDelay);
            auto dealAction = CallFunc::create([holder, card]() { holder->addCardToBack(card); });
            auto seq        = Sequence::create(delay, dealAction, nullptr);
            this->runAction(seq);
            totalDelay += delayPerCard;
        }
    }
}

void Deck::setColor(const ax::Color4F& color)
{
    if (_deckDrawNode)
    {
        _deckDrawNode->clear();
        Vec2 bottomLeft(-DECK_SIZE.x / 2, -DECK_SIZE.y / 2);
        Vec2 topRight(DECK_SIZE.x / 2, DECK_SIZE.y / 2);
        _deckDrawNode->drawSolidRect(bottomLeft, topRight, color);
    }
}

void Deck::connectHolder(Holder* holder)
{
    if (holder && std::find(_connectedHolders.begin(), _connectedHolders.end(), holder) == _connectedHolders.end())
    {
        _connectedHolders.push_back(holder);
    }
}

void Deck::connectHolder(const std::vector<Holder*>& holders)
{
    for (auto holder : holders)
    {
        connectHolder(holder);
    }
}

void Deck::disconnectHolder(Holder* holder)
{
    auto it = std::find(_connectedHolders.begin(), _connectedHolders.end(), holder);
    if (it != _connectedHolders.end())
    {
        _connectedHolders.erase(it);
    }
}

void Deck::disconnectHolder(const std::vector<Holder*>& holders)
{
    for (auto holder : holders)
    {
        disconnectHolder(holder);
    }
}

void Deck::setConnectedHolders(const std::vector<Holder*>& holders)
{
    _connectedHolders = holders;
}

const std::vector<Holder*>& Deck::getConnectedHolders() const
{
    return _connectedHolders;
}
