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
    enum class Rank
    {
        THREE = 0,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING,
        ACE,
        TWO
    };

    enum class Suit
    {
        SPADES = 0,
        CLUBS,
        DIAMONDS,
        HEARTS,
    };

public:

    bool init() override;
    void update(float delta) override;  

    // Player actions
    void onCardClicked(EventCard* event);

    bool isWinHand(const ax::Vector<Card*>& hand);
    bool isValidPlay(const std::set<Card*>& hand);   

protected:

    bool _clockWise                            = false;
    // state
    ax::Vector<Card*> _cards;
    ax::Vector<Zone*> _playerHands;  // represent player's hand
    Zone* _discardPile = nullptr;    // represent the discard pile
    Deck* _deck        = nullptr;    // represent the deck
    bool _isNewRound = true;
    vector<int> _previousPlayedCards;  // card store by value = rank* 4 + suit

    EventListenerCard* _cardListener = nullptr;

    Rank _currentRank = Rank::THREE;
    Suit _currentSuit = Suit::CLUBS;

    ax::ui::Text* _gameInfo = nullptr;
    ax::ui::Button* _playButton = nullptr;
    ax::ui::Button* _passButton = nullptr;

    // A game rule class will let all states access ít private members
    friend class TienLenState;
    friend class TienLenSetUpState;
    friend class TienLenDealState;
    friend class TienLenPlayState;
    friend class TienLenResultState;
};
