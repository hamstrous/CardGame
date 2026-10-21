#pragma once

#include "axmol.h"

class IContext;

class IState
{
public:
    IState() = default;
    IState(IContext* context) : _context(context) {}
    virtual ~IState() {}

    virtual void onEnter() = 0;
    virtual void onUpdate(float delta) = 0;
    virtual void onExit()  = 0;

    virtual void setContext(IContext* context) { _context = context; }
    IContext* getContext() { return _context; }

protected:
    IContext* _context = nullptr;
};
