#pragma once

#include "axmol.h"

#include <map>
#include <vector>

class Rule : public ax::Node
{
public:
    Rule();
    ~Rule() {}
    void update(float delta) override;

protected:
    int _clientId = 0;
    int _playerCount = 0;

    std::string _userName = "";
};
