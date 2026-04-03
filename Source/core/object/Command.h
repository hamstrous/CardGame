#pragma once

#include "axmol.h"

class Command
{
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
};


