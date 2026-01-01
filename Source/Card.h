#pragma once

#include "axmol.h"
#include "DraggableObject.h"

class Card : public DraggableObject
{
public:
    static Card* create(const std::string& frontTexture, const std::string& backTexture);
    Card* clone() const;

    virtual bool init(const std::string& frontTexture, const std::string& backTexture);

    void flip(float duration = 0.3f);
    void setFaceUp(bool faceUp);
    bool isFaceUp() const { return _isFaceUp; }

    void zoomToCenter(const ax::Vec2& screenCenter, float zoomScale = 5.0f, float duration = 0.3f);
    void unzoom(float duration = 0.3f);

    // Set card configuration from config file
    void setConfig(int id, float posX, float posY, float sizeX, float sizeY, float rotation);

    static const ax::Vec2 CARD_SIZE;
    static const int MOVE_TO_HOLDER_TAG;

private:
    ax::Sprite* _frontSprite = nullptr;
    ax::Sprite* _backSprite  = nullptr;
    bool _isFaceUp           = false;
};
