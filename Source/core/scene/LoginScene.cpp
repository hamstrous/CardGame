#include "LoginScene.h"
#include "network/HttpClient.h"

using namespace ax;
using namespace ax::network;
using namespace std;

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

    auto request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl("http://localhost:5284");

    request->setCompleteCallback([](HttpClient* client, HttpResponse* response) {
        if (response->getResponseCode() == 200)
        {
            auto* data = response->getResponseData();
            std::string body(data->data(), data->size());
            AXLOG("Player data: %s", body.c_str());
            // Parse JSON and update UI here
        }
        else
        {
            AXLOG("HTTP error: %d", response->getResponseCode());
        }
    });

    HttpClient::getInstance()->send(request);
    request->release();  // send() retains internally

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

void LoginScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {}

void LoginScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {}

LoginScene::~LoginScene() {}
