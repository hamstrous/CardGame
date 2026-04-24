#pragma once

#include "axmol.h"

#include <string>

//global game state that everything can access

class Player{
public:
    Player(const std::string& name, int index);

    void setName(const std::string& name) { _name = name; }
    std::string getName() const { return _name; }
    void setIndex(int index) { _index = index; }
    int getIndex() const { return _index; }             

private:
    std::string _name;
    int _index;
};

