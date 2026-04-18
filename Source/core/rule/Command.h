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
    virtual bool isExecuting() const { return _executing; }
    virtual void setExecuted(bool executed) { _executed = executed; }
    virtual void setExecuting(bool executing) { _executing = executing; }

protected:
    bool _executed = false;
    bool _executing = false;  // Flag to indicate if the command is currently executing
};


