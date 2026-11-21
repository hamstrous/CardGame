#pragma once

#include "axmol.h"

#include "core/network/SocketNetworkManager.h"
#include "core/event/EventListenerWebsocket.h"

#include "core/interface/IContext.h"

#include <map>
#include <vector>

class GameState;

class Rule : public ax::Node, public IContext
{
public:
    bool init() override;
    virtual bool onlineInit();

    Rule();
    ~Rule() {}
    void update(float delta) override;

    // mouse
    virtual bool onMouseDown(ax::Event* event);
    virtual bool onMouseUp(ax::Event* event);
    virtual bool onMouseMove(ax::Event* event);
    virtual bool onMouseScroll(ax::Event* event);

    // Keyboard
    virtual void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    virtual void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // WebSocket
    virtual void onWebSocketMessage(EventWebSocket* event);

    // Getters and Setters
    void setPlayerId(int playerId);
    int getPlayerId() const { return _clientPlayerId; }
    void setPlayerCount(int playerCount);
    int getPlayerCount() const { return _playerCount; }
    void setUserName(const std::string& userName);
    std::string getUserName() const { return _userName; }

    GameState* getCurrentState();

protected:

    bool _isHost     = false;
    int _clientPlayerId = 0;
    int _playerCount = 0;
    int _currentPlayerId  = 0;
    std::string _userName = "";

    SocketNetworkManager* _socketManager = nullptr;
   
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    EventListenerWebSocket* _websocketListener   = nullptr;
    int _sceneID                                 = 0;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;
};
