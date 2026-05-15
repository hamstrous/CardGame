#pragma once

#include "axmol.h"

#include "core/rule/GameState.h"

#include "UnoRule.h"

class UnoState : public GameState
{
public:
    UnoState(UnoRule* context) : GameState(context) {}

    UnoRule* getContext() { return static_cast<UnoRule*>(_context); }

    UnoRule::Value convertStringToUnoValue(const std::string& str);
    UnoRule::Color convertStringToUnoColor(const std::string& str);

    bool isCardSpecial(Card* card);

protected:
};
