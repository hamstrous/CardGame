#pragma once

#include "axmol.h"

#include <string>

#include "CardData.h"

#include "utils/helper.h"
#include "utils/Timer.hpp"

class Card : public ax::Node
{

public:

    static Card* create(CardData* property);
    bool init(CardData* property);

    bool onMouseDown(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseUp(ax::Event* event);

    void setContentSize(const ax::Size& contentSize) override;

    virtual void flip(float duration = 1.f);
    virtual void reveal();
    virtual void hide();

    void setDraggable(bool draggable);
    bool getDraggable();
    void setFaceUp(bool faceUp);
    bool getFaceUp();

    ~Card() override;

protected:
    lib::Timer _timer = lib::Timer(false);

    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;

    CardData* _property = new CardData();
    ax::Sprite* _frontSprite = nullptr;
    ax::Sprite* _backSprite = nullptr;

    //alias for property
    bool& _isFaceUp = _property->isFaceUp;
    bool& _isDraggable = _property->isDraggable;

    const int FLIP_ACTION_TAG = 1001;
    
};

