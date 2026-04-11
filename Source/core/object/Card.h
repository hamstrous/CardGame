#pragma once

#include "axmol.h"

#include <string>

#include "CardData.h"

#include "utils/helper.h"
#include "utils/Timer.hpp"

class Zone;

class Card : public ax::Node
{

public:

    // Factory method
    static Card* create(CardData* property);
    bool init(CardData* property);

    // Event handlers
    bool onMouseDown(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    // Overrides
    void setContentSize(const ax::Size& contentSize) override;

    // Actions
    virtual void flip(float duration = 1.f);
    virtual void reveal();
    virtual void hide();

    // Getters and Setters
    void setDraggable(bool draggable);
    bool getDraggable();
    void setFaceUp(bool faceUp);
    bool getFaceUp();

    // Movement
    void moveToPosition(const ax::Vec2& position, float duration = 1.f);
    void moveToZone(const Zone* targetZone, float duration = 1.f);

    // Constructor and Destructor
    ~Card() override;

protected:
    lib::Timer _clicktimer = lib::Timer(false);  // time elapsed since last click but not yet moved
    ax::Vec2 _dragOffset;
    bool _isDragging = false;

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;

    CardData* _property = new CardData();
    ax::Sprite* _frontSprite = nullptr;
    ax::Sprite* _backSprite = nullptr;

    Zone* _currentZone = nullptr;

    //alias for property
    bool& _isFaceUp = _property->isFaceUp;
    bool& _isDraggable = _property->isDraggable;

    const int FLIP_ACTION_TAG = 1001;
    
};

