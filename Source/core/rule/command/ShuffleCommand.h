#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/rule/Command.h"


class ShuffleCommand : public Command
{
public:
    ShuffleCommand(ax::Vector<Card*> &cards);
    virtual ~ShuffleCommand() {};
    void execute() override;
protected:
    ax::Vector<Card*> _cardsToShuffle;  // Cards to be shuffled
};


