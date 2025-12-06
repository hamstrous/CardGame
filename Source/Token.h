#pragma once

#include "axmol.h"
#include "DraggableObject.h"

class Token : public DraggableObject
{
public:
    static Token* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    static const ax::Vec2 TOKEN_SIZE;

private:
    ax::Sprite* _sprite = nullptr;
};
