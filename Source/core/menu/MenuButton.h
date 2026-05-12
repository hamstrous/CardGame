#pragma once

#include "axmol.h"
#include "ui/UIButton.h"
#include "core/scene/GameScene.h"
#include "core/scene/RoomScene.h"

#include "core/rule/Rule.h"

#include <string>

class MenuButton : public ax::ui::Button
{
public:
    template <typename T = Rule>
    static MenuButton* create(const std::string& imagePath, const std::string& buttonName)
    {
        MenuButton* menuButton = new (std::nothrow) MenuButton();
        if (menuButton && menuButton->init<T>(imagePath, buttonName))
        {
            menuButton->autorelease();
            return menuButton;
        }
        AX_SAFE_DELETE(menuButton);
        return nullptr;
    }

    template <typename T = Rule>
    bool init(const std::string& imagePath, const std::string& buttonName)
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
        // addChild(_buttonLabel);
        this->setSwallowMouse(true);
        this->addClickEventListener([this](ax::Object* sender) {
            auto director = ax::Director::getInstance();
            auto gameRule    = ax::utils::createInstance<T>();

            auto roomScene = ax::utils::createInstance<RoomScene>();
            AXASSERT(gameRule != nullptr, "Game rule created by button is null");

            GameScene::getInstance()->setRule(gameRule);

            director->replaceScene(roomScene);
            return true;
        });
        return true;
    }


private:
    ax::Sprite* _buttonSprite = nullptr;
    ax::Label* _buttonLabel = nullptr;

};
