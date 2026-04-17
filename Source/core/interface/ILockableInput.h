#pragma once

class ILockableInput
{

public:
    // Important methods
    virtual void lockInput() = 0;
    virtual void unlockInput() = 0;

    virtual ~ILockableInput() = default;
    
};

