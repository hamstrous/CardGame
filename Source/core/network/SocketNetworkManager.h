#pragma once

#include "axmol.h"
#include "network/WebSocket.h"

#include <string>
#include <map>

using namespace std;
using namespace ax;
using namespace ax::network;

class SocketNetworkManager : public WebSocket::Delegate
{

public:
    SocketNetworkManager();

    void onOpen(WebSocket* ws) override;
    void onMessage(WebSocket* ws, const WebSocket::Data& data) override;
    void onClose(WebSocket* ws, uint16_t code, std::string_view reason) override;
    void onError(WebSocket* ws, const WebSocket::ErrorCode& error) override;

    void connect(const std::string& url);
    void sendMessage(const std::string& message);

    void setAuthorizationHeader(const std::string& authToken);

private:
    WebSocket* _ws;
};
