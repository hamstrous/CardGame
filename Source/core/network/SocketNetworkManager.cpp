#include "SocketNetworkManager.h"
#include "utils/json.hpp"
#include "core/event/EventWebSocket.h"

using json = lib::json;

SocketNetworkManager::SocketNetworkManager()
{
    _ws = new WebSocket();
}

void SocketNetworkManager::onOpen(WebSocket* ws)
{
    AXLOGD("WebSocket connection opened.");
    ws->send("{\"cmd\":\"join_room\",\"room_id\":\"123\"}");
}
void SocketNetworkManager::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    if (data.isBinary)
    {

    }
    else
    { // JSON
        std::string message(data.bytes, data.len);
        AXLOGD("Received message: {}", message);
        json jsonMessage = json::parse(message);
        std::string cmd  = jsonMessage["cmd"];

        EventWebSocket *event = new EventWebSocket(cmd, jsonMessage);
        ax::Director::getInstance()->getEventDispatcher()->dispatchEvent(event);
    }
}
void SocketNetworkManager::onClose(WebSocket* ws, uint16_t code, std::string_view reason)
{
    AXLOGD("WebSocket connection closed.");
}
void SocketNetworkManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    switch (error)
    {
    case WebSocket::ErrorCode::TIME_OUT:
        AXLOGD("WebSocket error: Time out");
        break;
    case WebSocket::ErrorCode::CONNECTION_FAILURE:
        AXLOGD("WebSocket error: Connection failure");
        break;
    default:
        AXLOGD("WebSocket error");
    }
}

void SocketNetworkManager::connect(const std::string& url)
{
    _ws->open(this, url);
}

void SocketNetworkManager::sendMessage(const std::string& message)
{
    if (_ws && _ws->getReadyState() == WebSocket::State::OPEN)
    {
        _ws->send(message);
    }
}

void SocketNetworkManager::setAuthorizationHeader(const std::string& authToken)
{
    _ws->setHeaders({"Authorization:" + authToken});
}
