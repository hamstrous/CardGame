#pragma once

#include "axmol.h"

#include "core/rule/GameState.h"

#include "TienLenRule.h"

class TienLenState : public GameState
{
public:
    TienLenState(TienLenRule* context) : GameState(context) {}

    TienLenRule* getContext() { return static_cast<TienLenRule*>(_context); }

    TienLenRule::Value convertStringToTienLenValue(const std::string& str);
    TienLenRule::Color convertStringToTienLenColor(const std::string& str);

    virtual void onCardClicked(EventCard* event) {};

    bool isCardSpecial(Card* card);

protected:
};
