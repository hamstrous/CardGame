#include "Token.h"
using namespace ax;

const ax::Vec2 TOKEN_SIZE = ax::Vec2(64.0f, 64.0f);

Token* Token::create(const std::string& texture)
{
    Token* token = new (std::nothrow) Token();
    if (token && token->init(texture))
    {
        token->autorelease();
        return token;
    }
    AX_SAFE_DELETE(token);
    return nullptr;
}

bool Token::init(const std::string& texture)
{
    if (!Node::init())
        return false;

    _objectSize = TOKEN_SIZE;

    _sprite = Sprite::create(texture);

    if (!_sprite)
        return false;

    _sprite->setContentSize(TOKEN_SIZE);
    this->addChild(_sprite);
    setContentSize(TOKEN_SIZE);
    return true;
}
