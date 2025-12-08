#pragma once

#include "axmol.h"
#include "DraggableObject.h"
#include "ui/UIButton.h"

class Counter : public DraggableObject
{
public:
    static Counter* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    static const ax::Vec2 COUNTER_SIZE;
    static const ax::Vec2 COUNTER_BUTTON_SIZE_PERCENT;

private:
    int _count = 0;
    ax::Sprite* _counterSprite = nullptr;
    ax::ui::Button* _incrementButton = nullptr;
    ax::ui::Button* _decrementButton = nullptr;
    ax::ui::Button* _resetButton     = nullptr;
    ax::Label* _countLabel = nullptr;
};
