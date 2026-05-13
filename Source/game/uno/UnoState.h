#pragma once

#include "axmol.h"

#include "core/rule/GameState.h"

#include "UnoRule.h"


class UnoState : public GameState
{
public:
    UnoRule* getContext() { return static_cast<UnoRule*>(_context); }

protected:
};
