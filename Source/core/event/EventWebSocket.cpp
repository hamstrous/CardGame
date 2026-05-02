#include "EventWebSocket.h"
#include "EventListenerWebSocket.h"

EventWebSocket::EventWebSocket(const std::string& cmd, const json& data)
    : EventCustom(cmd)
    , _data(data)
{}

