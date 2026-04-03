#pragma once

#include "axmol.h"

#include <string>

#include "CardProperty.h"

class Card : public ax::Node
{
public:

    static Card* create(const CardProperty* property);
    bool init(const CardProperty* property);

    void setContentSize(const ax::Size& contentSize) override;


    ~Card() override;

protected:
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    CardProperty* _property;

    ax::Sprite* _frontSprite;
    ax::Sprite* _backSprite;
    
};

