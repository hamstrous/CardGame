#pragma once

#include "axmol.h"
#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/rule/Command.h"

class DealCommand : public Command
{
public:
    DealCommand(const ax::Vector<Card*>& cardsToDeal, const ax::Vector<Zone*>& targetZones)
        : _cardsToDeal(cardsToDeal), _targetZones(targetZones)
    {}
    void execute() override;
    static const int DEAL_COMMAND_TAG = 3000;

protected:
    ax::Vector<Card*> _cardsToDeal;  // Cards to be dealt
    ax::Vector<Zone*> _targetZones;  // Target zones for the cards

      // Flag to track if the command has been executed
};


