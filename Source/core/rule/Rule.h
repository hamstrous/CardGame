#pragma once

#include "axmol.h"

#include "core/network/SocketNetworkManager.h"
#include "core/event/EventListenerWebsocket.h"

#include "core/interface/IContext.h"
#include "core/rule/GameState.h"

#include <map>
#include <vector>

class Rule : public ax::Node, public IContext
{
public:
    bool init() override;
    virtual bool onlineInit();

    Rule();
    ~Rule() {}
    void update(float delta) override;

    // mouse
    bool onMouseDown(ax::Event* event);
    bool onMouseUp(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseScroll(ax::Event* event);

    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // WebSocket
    void onWebSocketMessage(EventWebSocket* event);

    // Getters and Setters
    void setPlayerId(int playerId);
    int getPlayerId() const { return _playerId; }
    void setPlayerCount(int playerCount);
    int getPlayerCount() const { return _playerCount; }
    void setUserName(const std::string& userName);
    std::string getUserName() const { return _userName; }

    GameState* getCurrentState() const { return static_cast<GameState*>(_currentState); }

protected:

    int _playerId = 0;
    int _playerCount = 0;

    std::string _userName = "";

    SocketNetworkManager* _socketManager = nullptr;
};
