#pragma once

#include "axmol.h"

#include <string>
#include "utils/json.hpp"

using json = lib::json;

class AX_DLL EventWebSocket : public ax::EventCustom
{
public:
    EventWebSocket(const std::string& cmd, const json& data);

private:
    json _data;
    
    friend class EventListenerWebSocket;
};

