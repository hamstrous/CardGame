#include "MenuButton.h"

MenuButton* MenuButton::create(const std::string& imagePath, const std::string& buttonName)
{
    MenuButton* menuButton = new (std::nothrow)  MenuButton();
    if (menuButton && menuButton->init(imagePath, buttonName))
    {
        menuButton->autorelease();
        return menuButton;
    }
    AX_SAFE_DELETE(menuButton);
    return nullptr;
}

bool MenuButton::init(const std::string& imagePath, const std::string& buttonName)
{
    if (!Button::init(imagePath))
    {
        return false;
    }
    _buttonLabel = ax::Label::createWithSystemFont(buttonName, "Arial", 24);
    /*_buttonSprite = ax::Sprite::create(imagePath);
    if (_buttonSprite == nullptr || _buttonLabel == nullptr)
    {
        return false;
    }
    addChild(_buttonSprite);*/
    this->setTitleLabel(_buttonLabel);
    //addChild(_buttonLabel);
    this->setSwallowMouse(true);
    this->addClickEventListener([this](ax::Object* sender) {
        AXLOG("%s selected", _buttonLabel->getString().data());
        return true;
    });
    return true;
}
