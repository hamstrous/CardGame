#pragma once

#include "axmol.h"

class Command : public ax::Node
{
public:
    virtual ~Command() {}
    virtual void execute() const;
    virtual void undo() const;
    virtual ax::Action* getAction() const;  // Get the action associated with this command
};


