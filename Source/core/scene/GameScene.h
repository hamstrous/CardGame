#pragma once

#include "axmol.h"

#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/event/EventListenerZone.h"

#include "core/rule/Rule.h"


class GameScene : public ax::Scene
{
public:
    // must be set before enabling
    void setRule(Rule* rule) {
        _rule = rule;
        this->addChild(_rule);
        _rule->setPosition(ax::Vec2::ZERO);
        AXLOGD("Rule set in GameScene, needs rule initialization next");
    }

    void initRule()
    {
        if (_rule != nullptr)
        {
            // All the necessary information for the rule to function should be set before calling this method
            _rule->setPlayerId(_playerId);
            _rule->setPlayerCount(_playerCount);
            _rule->setUserName(_userName);
            AXLOGD("Rule initialized, game is ready");
        }
    }

public:
    // Singleton pattern
    // Only one instance of GameScene can exist at a time, and it can be accessed globally for convenience
    static GameScene* getInstance();
    static GameScene* _instance;

    bool init();
    void update(float delta) override;

    // mouse
    bool onMouseDown(ax::Event* event);
    bool onMouseUp(ax::Event* event);
    bool onMouseMove(ax::Event* event);
    bool onMouseScroll(ax::Event* event);

    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    void setUserName(std::string userName);
    std::string getUserName();
    void setRoomId(std::string roomId);
    std::string getRoomId();
    void setPlayerCount(int playerCount);
    int getPlayerCount();
    void setPlayerId(int playerId);
    int getPlayerId();

    void onEnter() override;

    ~GameScene() override;

protected:
    ax::EventListenerKeyboard* _keyboardListener = nullptr;
    ax::EventListenerMouse* _mouseListener       = nullptr;
    int _sceneID                                 = 0;

    Rule* _rule = nullptr;

    ax::Vec2 visibleSize = _director->getVisibleSize();
    ax::Vec2 origin      = _director->getVisibleOrigin();
    ax::Rect safeArea    = _director->getSafeAreaRect();
    ax::Vec2 safeOrigin  = safeArea.origin;

    int _playerId    = 0;
    int _playerCount = 0;

    std::string _userName = "";
    std::string _roomId   = "";

    //EventListenerZone* _cardEventListener = nullptr;
};
