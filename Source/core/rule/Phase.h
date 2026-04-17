#pragma once

#include "axmol.h"
#include "Command.h"

class Phase
{
public:
    Phase()        = default;
    virtual void execute() = 0;

protected:
    std::vector<Command*> _commandList;    
};
