#pragma once

#include "axmol.h"

#include "IContext.h"

class IState
{
public:
    IState() = default;
    IState(IContext* context) : _context(context) {}
    virtual ~IState() {}

    virtual void onEnter() = 0;
    virtual void onUpdate(float delta) = 0;
    virtual void onExit()  = 0;

protected:
    IContext* _context = nullptr;
};
