#pragma once

#include "axmol.h"

#include "ui/UIText.h"
#include "core/event/EventListenerWebSocket.h"

class LobbyScene : public ax::Scene
{

public:
    bool init() override;

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

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    void onEnter() override;

    void startSocket(std::string authToken);

    ~LobbyScene() override;

protected:
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    EventListenerWebSocket* _websocketListener   = nullptr;
    int _sceneID                                 = 0;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    ax::Vector<ax::Label*> _usersInRoom;
    ax::Label* _roomIdText = nullptr;
};
