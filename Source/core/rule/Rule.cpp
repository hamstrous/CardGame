#include "Rule.h"

bool Rule::init()
{
    return true;
}

bool Rule::onlineInit()
{
    return false;
}


Rule::Rule() {

}

void Rule::update(float delta) {

}

void Rule::setPlayerId(int playerId) {
    _playerId = playerId;
}

void Rule::setPlayerCount(int playerCount) {
    _playerCount = playerCount;
}

void Rule::setUserName(const std::string& userName) {
    _userName = userName;
}
