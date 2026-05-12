#pragma once

#include "axmol.h"

#include "core/network/SocketNetworkManager.h"

#include <map>
#include <vector>

class Rule : public ax::Node
{
public:
    bool init() override;
    virtual bool onlineInit();

    Rule();
    ~Rule() {}
    void update(float delta) override;

    void setPlayerId(int playerId);
    void setPlayerCount(int playerCount);
    void setUserName(const std::string& userName);

protected:
    int _playerId = 0;
    int _playerCount = 0;

    std::string _userName = "";

    SocketNetworkManager* _socketManager = nullptr;
};
