#pragma once

#include "axmol.h"

class Command : public ax::Node
{
public:
    virtual ~Command() {}
    virtual void execute() {};
    virtual void undo() {};
    virtual ax::Action* getAction() { return nullptr; };  // Get the action associated with this command
    virtual bool isExecuted() const { return _executed; }
    virtual void setExecuted(bool executed) { _executed = executed; }

protected:
    bool _executed = false;
};


