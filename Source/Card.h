#pragma once

#include "axmol.h"

class Card : public ax::Node
{
public:
    static Card* create(const std::string& frontTexture, const std::string& backTexture);

    virtual bool init(const std::string& frontTexture, const std::string& backTexture);
   
    void flip(float duration = 0.3f);
    void setFaceUp(bool faceUp);
    bool isFaceUp() const { return _isFaceUp; }

    bool containsPoint(const ax::Vec2& worldPoint) const;

    bool isDragging() const { return _isDragging; }
    void enableDragging(bool enable);

    void setDragOffset(const ax::Vec2& offset) { _dragOffset = offset; }
    ax::Vec2 getDragOffset() const { return _dragOffset; }

    void setOriginalPosition(const ax::Vec2& pos) { _originalPosition = pos; }
    ax::Vec2 getOriginalPosition() const { return _originalPosition; }

    void startDragging() { _isDragging = true; }
    void stopDragging() { _isDragging = false; }

    void setHighlight(bool highlight);
private:
    const ax::Vec2 CARD_SPRITE_OFFSET = ax::Vec2(15,12);
    const ax::Vec2 CARD_SIZE = ax::Vec2(64,96);

    ax::Sprite* _frontSprite = nullptr;
    ax::Sprite* _backSprite = nullptr;
    bool _isFaceUp = false;
    bool _isDraggable = true;
    bool _isDragging = false;

    ax::Vec2 _dragOffset;         // Offset between mouse and card center
    ax::Vec2 _mouseDownPosition;  // Position where mouse was pressed

    ax::Vec2 _originalPosition;

    float _originalScale = 1.0f;
    ax::DrawNode* _highlightNode;
    ax::DrawNode* _cardBox;

    ax::Rect* _boundingBox;
};
