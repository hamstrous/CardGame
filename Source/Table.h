#pragma once

#include "axmol.h"
#include "Card.h"

class Table : public ax::Node
{
public:
    static Table* create(const std::string& texture);

    virtual bool init(const std::string& texture);

    void addCard(Card* card);
    void removeCard(Card* card);
    void addCardAt(Card* card, int index);
    void moveCardToPosition(Card* card, const ax::Vec2& position);

    bool containsPoint(const ax::Vec2& worldPoint) const;

    //void setCardSpacing(float spacing) { _cardSpacing = spacing; }
    void setMaxSpacing(float maxSpacing) { _maxCardSpacing = maxSpacing; }

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
    const ax::Vec2 TABLE_SIZE = ax::Vec2(300,140);

    ax::Sprite* _tableSprite = nullptr;

    bool _isDraggable = true;
    bool _isDragging = false;

    std::vector<Card*> _cards;
    const float _cardSpacing = 40;  
    float _maxCardSpacing;  

    ax::Vec2 getCardPosition(int index, int cardCount) const;

    ax::Vec2 _dragOffset;         // Offset between mouse and card center
    ax::Vec2 _mouseDownPosition;  // Position where mouse was pressed

    ax::Vec2 _originalPosition;

    float _originalScale = 1.0f;
    ax::DrawNode* _highlightNode;

    ax::Rect* _boundingBox;
};
