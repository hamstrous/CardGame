#pragma once

#include "axmol.h"

#include "ui/UIButton.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "core/network/SocketNetworkManager.h"
#include "core/event/EventListenerWebsocket.h"

class RoomScene : public ax::Scene
{

public:
    bool init() override;

    void setUpObjects();
    void setUpRule();

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

    void onCreateRoomMessage(EventWebSocket* event);
    void onJoinRoomMessage(EventWebSocket* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    void startSocket(std::string authToken);

    ~RoomScene() override;

protected:
    EventListenerWebSocket* _websocketListener   = nullptr;
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    int _sceneID                                 = 0;

    ax::ui::Button* _createRoomButton = nullptr;
    ax::ui::Button* _joinRoomButton = nullptr;

    ax::ui::EditBox* _roomIdEditBox = nullptr;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    SocketNetworkManager* _socketManager;

};
