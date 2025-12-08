#include "Table.h"
#include <limits>
#include <algorithm>
#include "Counter.h"
using namespace ax;

const ax::Vec2 Counter::COUNTER_SIZE{100.f, 100.f};
const ax::Vec2 Counter::COUNTER_BUTTON_SIZE_PERCENT{0.4f, 1 / 3.f};

Counter* Counter::create(const std::string& texture)
{
    Counter* counter = new (std::nothrow) Counter();
    if (counter && counter->init(texture))
    {
        counter->autorelease();
        return counter;
    }
    AX_SAFE_DELETE(counter);
    return nullptr;
}

bool Counter::init(const std::string& texture)
{
    if (!Node::init())
        return false;

    _objectSize  = COUNTER_SIZE;
    _isDraggable = false;

    _counterSprite = Sprite::create(texture);

    if (!_counterSprite)
    {
        AXLOG("Failed to load counter texture: %s", texture.c_str());
        return false;
    }

    _counterSprite->setContentSize(COUNTER_SIZE);
    this->addChild(_counterSprite);
    setContentSize(COUNTER_SIZE);

    _incrementButton = ui::Button::create("ui/increase.png");
    _decrementButton = ui::Button::create("ui/decrease.png");
    _resetButton     = ui::Button::create("ui/reset.png");

    ax::Vec2 buttonSize = COUNTER_SIZE * COUNTER_BUTTON_SIZE_PERCENT;
    _incrementButton->setScale9Enabled(true);
    _incrementButton->setCapInsets(ax::Rect(0, 0, 16, 16));
    _decrementButton->setScale9Enabled(true);
    _decrementButton->setCapInsets(ax::Rect(0, 0, 16, 16));
    _resetButton->setScale9Enabled(true);
    _resetButton->setCapInsets(ax::Rect(0, 0, 16, 16));

    _incrementButton->setContentSize(buttonSize);
    _decrementButton->setContentSize(buttonSize);
    _resetButton->setContentSize(buttonSize);

    // set all the button to the right of counter sprite
    _incrementButton->setPosition(ax::Vec2(COUNTER_SIZE.x + buttonSize.x / 2, buttonSize.y / 2 * 5));
    _decrementButton->setPosition(ax::Vec2(COUNTER_SIZE.x + buttonSize.x / 2, buttonSize.y / 2 * 3));
    _resetButton->setPosition(ax::Vec2(COUNTER_SIZE.x + buttonSize.x / 2, buttonSize.y / 2));

    _counterSprite->addChild(_incrementButton);
    _counterSprite->addChild(_decrementButton);
    _counterSprite->addChild(_resetButton);

    // Use a generic lambda to avoid depending on `ax::Ref` type name
    _incrementButton->addClickEventListener([this](auto /* sender */) {
        _count += 1;
        if (_countLabel)
            _countLabel->setString(std::to_string(_count));
    });

    _decrementButton->addClickEventListener([this](auto /* sender */) {
        _count -= 1;
        if (_countLabel)
            _countLabel->setString(std::to_string(_count));
    });

    _resetButton->addClickEventListener([this](auto /* sender */) {
        _count = 0;
        if (_countLabel)
            _countLabel->setString(std::to_string(_count));
    });

    _countLabel = ax::Label::createWithSystemFont("0", "Arial", 24);
    // center the label inside the sprite
    _countLabel->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
    _countLabel->setPosition(_counterSprite->getContentSize() * 0.5f);
    _counterSprite->addChild(_countLabel);

    return true;
}
