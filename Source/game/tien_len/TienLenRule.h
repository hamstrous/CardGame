#pragma once

#include "axmol.h"

#include "core/event/EventListenerWebsocket.h"
#include "core/event/EventListenerCard.h"
#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/object/Deck.h"

#include "ui/UIButton.h"

#include "core/event/EventCard.h"

#include "ui/UIText.h"

#include "core/rule/Rule.h"

class TienLenRule : public Rule
{
public:
    enum class Color
    {
        RED,
        YELLOW,
        GREEN,
        BLUE,
        WILD
    };

    enum class Value
    {
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        SKIP,
        REVERSE,
        DRAW_TWO,
        WILD,
        WILD_DRAW_FOUR
    };

    enum class Phase
    {
        WAIT,
        SHUFFLE,
        DEAL,
        PLAY,
        RESULT
    };

public:

    bool init() override;
    void update(float delta) override;  

    // Player actions
    void onCardClicked(EventCard* event);

protected:

    bool _clockWise                            = true;
    // state
    ax::Vector<Card*> _cards;
    int _plus2StackCount = 0;  // count how many +2 cards have been stacked
    bool _skipNextPlayer = false;    // whether the next player's turn should be skipped
    bool _plus4Stacked   = false;    // whether a +4 card has been played and not yet resolved
    ax::Vector<Zone*> _playerHands;  // represent player's hand
    Zone* _discardPile = nullptr;    // represent the discard pile
    Deck* _deck        = nullptr;    // represent the deck

    ax::Vector<ax::ui::Button*> _colorButtons;  // buttons for choosing color when playing wild cards

    EventListenerCard* _cardListener = nullptr;

    Color _currentColor = Color::RED;
    Value _currentValue = Value::ZERO;

    ax::ui::Text* _gameInfo = nullptr;

    // A game rule class will let all states access ít private members
    friend class TienLenState;
    friend class TienLenSetUpState;
    friend class TienLenDealState;
    friend class TienLenPlayState;
    friend class TienLenResultState;
};
