#include "Piece.h"
#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include "core/singleton/InputManager.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace helper;

int Piece::_count = 0;

Piece* Piece::create(std::string type)
{
    Piece* piece = new (std::nothrow) Piece();
    if (piece && piece->init(type))
    {
        piece->_id = ++_count;
        piece->autorelease();
        return piece;
    }
    AX_SAFE_DELETE(piece);
    return nullptr;
}

bool Piece::init(std::string type)
{
    if (!Node::init())
    {
        return false;
    }  

    _mouseListener = ax::EventListenerMouse::create();
    _mouseListener->onMouseDown = AX_CALLBACK_1(Widget::onMouseDown, this);
    _mouseListener->onMouseMove = AX_CALLBACK_1(Widget::onMouseMove, this);
    _mouseListener->setSwallowMouse(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    setUpInteractEffect();

    scheduleUpdate();
    return true;
}

void Piece::update(float delta) {
    InputManager* inputManager = InputManager::getInstance();
    inputManager->isWidgetIntersectingRubberBand(this);
    
}
