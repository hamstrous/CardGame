#pragma once

#include "axmol.h"

class Command : public ax::Node
{
public:
    virtual ~Command() {}
    virtual void execute() {};
    virtual void undo() {};
    virtual ax::Action* getAction() { return nullptr; };  // Get the action associated with this command
    virtual bool isDone() const { return _isDone; }
    virtual bool isRunning() const { return _isRunning; }
    virtual void setDone(bool isDone)
    {
        _isDone = isDone;
        if (_isDone && _onCompleteCallback)
        {
            _onCompleteCallback();
        }
    }
    virtual void setRunning(bool isRunning) { _isRunning = isRunning; }

    void setOnCompleteCallback(std::function<void()> callback) { _onCompleteCallback = callback; }

protected:
    bool _isDone = false;
    bool _isRunning = false; 
    std::function<void()> _onCompleteCallback = nullptr;  // Callback to be called when the command is complete
};


