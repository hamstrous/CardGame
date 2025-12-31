#pragma once

#include "axmol.h"
#include "DraggableObject.h"
#include "ui/UIButton.h"

class Counter : public DraggableObject
{
public:
    static Counter* create(const std::string& texture);

    virtual bool init(const std::string& texture);
    Counter* clone() const;

    // Set counter configuration from config file
    void setConfig(int id, float posX, float posY, float sizeX, float sizeY, float rotation, int startingValue);

    static const ax::Vec2 COUNTER_SIZE;
    static const ax::Vec2 COUNTER_BUTTON_SIZE_PERCENT;

private:
    int _count                       = 0;
    ax::Sprite* _counterSprite       = nullptr;
    ax::ui::Button* _incrementButton = nullptr;
    ax::ui::Button* _decrementButton = nullptr;
    ax::ui::Button* _resetButton     = nullptr;
    ax::Label* _countLabel           = nullptr;
};
