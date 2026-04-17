#pragma once

#include "axmol.h"

class Command : public ax::Node
{
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
    virtual void Undo() const;
    virtual ax::Action* getAction() const = 0;  // Get the action associated with this command
};


