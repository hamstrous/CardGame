#include "SocketNetworkManager.h"

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
    AXLOGD("WebSocket message received.");
}
void SocketNetworkManager::onClose(WebSocket* ws, uint16_t code, std::string_view reason)
{
    AXLOGD("WebSocket connection closed.");
}
void SocketNetworkManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    AXLOGD("WebSocket error occurred.");
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
