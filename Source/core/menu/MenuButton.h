#pragma once

#include "axmol.h"
#include "ui/UIButton.h"

#include <string>

class MenuButton : public ax::ui::Button
{
public:
    static MenuButton* create(const std::string& imagePath, const std::string& buttonName);
    bool init(const std::string& imagePath, const std::string& buttonName);

private:
    ax::Sprite* _buttonSprite = nullptr;
    ax::Label* _buttonLabel = nullptr;
};
