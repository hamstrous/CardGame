#include "Rule.h"
#include "GameState.h"

Rule* GameState::getContext()
{
    return static_cast<Rule*>(_context);
}
