#pragma once

#include "axmol.h"

#include "core/rule/GameState.h"

#include "TienLenRule.h"

class TienLenState : public GameState
{
public:
    TienLenState(TienLenRule* context) : GameState(context) {}

    TienLenRule* getContext() { return static_cast<TienLenRule*>(_context); }

    TienLenRule::Rank convertStringToTienLenRank(const std::string& str);
    TienLenRule::Suit convertStringToTienLenSuit(const std::string& str);

    int convertTienLenCardToValue(Card* card);

    virtual void onCardClicked(EventCard* event) {};

protected:
};
