#pragma once

#include "axmol.h"
#include "DraggableObject.h"

class Card : public DraggableObject
{
public:
    static Card* create(const std::string& frontTexture, const std::string& backTexture);

    virtual bool init(const std::string& frontTexture, const std::string& backTexture);

    void flip(float duration = 0.3f);
    void setFaceUp(bool faceUp);
    bool isFaceUp() const { return _isFaceUp; }

    inline static const ax::Vec2 CARD_SIZE{82, 127.5f};

private:
    ax::Sprite* _frontSprite = nullptr;
    ax::Sprite* _backSprite  = nullptr;
    bool _isFaceUp           = false;
};
