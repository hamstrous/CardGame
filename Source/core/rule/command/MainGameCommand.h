#pragma once

#include "axmol.h"

#include "core/object/Zone.h"
#include "core/rule/Command.h"
#include "core/interface/ILockableInput.h"
#include "core/event/EventListenerZone.h"

class MainGameCommand : public Command
{
public:
    MainGameCommand(Zone* playField);
    virtual ~MainGameCommand() {};
    void execute() override;
    void update(float delta) override;
    void onMainFieldCardReceived(EventZone* event);
    void setCurrentPlayerIndex(int index);

protected:
    std::vector<std::vector<ILockableInput*>> _playerList = std::vector<std::vector<ILockableInput*>>(2);
    EventListenerZone* _zoneListener = nullptr;
    Zone* _playField                 = nullptr;  // The main play field zone
    int _currentPlayerIndex          = 1;        // Index to track the current player
    bool _firstTime                                       = true;
};
