#include "LoginScene.h"

#include "network/HttpClient.h"
#include "core/network/HttpRequestHandler.h"
#include "core/network/SocketNetworkManager.h"

#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIButton.h"

#include <format>
#include "utils/json.hpp"

using namespace ax;
using namespace ax::network;
using namespace ax::ui;
using namespace std;
using json = lib::json;

bool LoginScene::init()
{
    // super init first
    if (!Scene::init())
    {
        return false;
    }

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(LoginScene::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(LoginScene::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(LoginScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(LoginScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(LoginScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    scheduleUpdate();

    ui::EditBox* usernameEditBox = EditBox::create(Size(200, 40), "background.png");
    usernameEditBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(usernameEditBox);
    ui::EditBox* passwordEditBox = EditBox::create(Size(200, 40), "background.png");    
    passwordEditBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));
    this->addChild(passwordEditBox);

    Button* loginButton = Button::create("background.png");
    loginButton->ignoreContentAdaptWithSize(false);
    loginButton->setContentSize(Size(100, 50));
    loginButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
    this->addChild(loginButton);

    loginButton->addClickEventListener([this, usernameEditBox, passwordEditBox](ax::Object* sender) {
        // Handle login button click
        // You can retrieve the username and password from the edit boxes and perform login logic here
        string loginBody = std::format("{{\"username\": \"{}\", \"password\": \"{}\"}}", usernameEditBox->getText(),
                                       passwordEditBox->getText());
        AXLOGD("Login button clicked, sending POST request with body: {}", loginBody);
        HttpRequestHandler::setJsonRequest(true);
        HttpRequestHandler::sendPostRequest("/login", loginBody,
                                            [this](HttpClient* client, HttpResponse* response) {
            if (response->isSucceed())
            {
                string responseStr = HttpRequestHandler::convertBufferToString(response->getResponseData());
                // Handle successful login
                json responseJson = json::parse(responseStr);
                AXLOGD("Login successful: {}", responseJson["auth_token"]);
                _socketManager = new SocketNetworkManager();
                _socketManager->setAuthorizationHeader(responseJson["auth_token"]);
                _socketManager->connect("ws://localhost:5284/ws");
                
            }
            else
            {
                // Handle login failure
                AXLOGD("Login failed");
            }
        });
        
    });

    return true;
}

bool LoginScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool LoginScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool LoginScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void LoginScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    if (code == EventKeyboard::KeyCode::KEY_J)
    {
        _socketManager->sendMessage("{\"cmd\": \"join_room\", \"room_id\": \"room\"}");
    }
}

void LoginScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

void LoginScene::startSocket(string authToken) {
    
}

LoginScene::~LoginScene() {}
