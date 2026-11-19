#include "Holder.h"

#include "ImGui/ImGuiPresenter.h"

#include "utils/helper.h"
#include "utils/ScreenHelper.h"
#include "utils/Collision.h"

#include "core/event/EventCard.h"
#include "core/const/GameConstants.h"

#include "core/singleton/InputManager.h"
#include "core/singleton/LayerManager.h"
#include "core/singleton/ModeManager.h"
#include "core/ui/GuiManager.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace helper;
using namespace ax;
using namespace std;

int Holder::_count = 0;

Holder* Holder::create()
{
    Holder* holder = new (std::nothrow) Holder();
    if (holder && holder->init())
    {
        holder->autorelease();
        return holder;
    }
    AX_SAFE_DELETE(holder);
    return nullptr;
}

// The layer that holder get added to is deterministic
void Holder::addToLayer()
{
    LayerManager::getInstance()->addWidgetToNonInteractiveLayer(this);
}

bool Holder::init()
{
    if (!Node::init())
    {
        return false;
    }  

    _mouseListener = ax::EventListenerMouse::create();
    _mouseListener->onMouseDown = AX_CALLBACK_1(Holder::onMouseDown, this);
    _mouseListener->onMouseMove = AX_CALLBACK_1(Holder::onMouseMove, this);
    _mouseListener->setSwallowMouse(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    setUpMainVisual();  //Before effect
    setUpInteractEffect();

    scheduleUpdate();

    _inspectorData = createInspectorData();

    _id = _count++;
    this->setName(getId());

    addAllowedRotation(0);
    addAllowedRotation(90);
    addAllowedRotation(180);
    addAllowedRotation(270);

    _faceCycle = 1;

    GuiManager::getInstance()->addCallbackToImGuiDrawLoop([this]() { drawGui(); });

    return true;
}

Holder* Holder::clone() const
{
    Holder* holder = Holder::create();
    holder->setContentSize(this->getContentSize());
    holder->setPosition(this->getPosition() + ax::Vec2(30, -30));
    holder->setScale(this->getScale());
    holder->setRotation(this->getRotation());
    return holder;
}

void Holder::drawGui()
{
    ImGui::DragFloat("Corner", &_roundedCorner, 0.5f);
}

void Holder::update(float delta) {
    InputManager* inputManager = InputManager::getInstance();
    inputManager->isWidgetIntersectingRubberBand(this);
}

//void Holder::setContentSize(const ax::Size& contentSize) {
//    Node::setContentSize(contentSize);
//    if (_image) {
//        _image->setContentSize(contentSize);
//    }
//}

void Holder::setUpMainVisual()
{
    _drawNodeVisual = ax::DrawNode::create();
    _drawNodeVisual->properties.setDrawOrder(true);
    this->addChild(_drawNodeVisual);

    //_image = Sprite::create("virtualtabletop assets/cards-default/1B.svg");
    _image = Sprite::create();
    this->addChild(_image);

    schedule([this](float delta) {
        float lineThickness = 5.f;
        float curveThickness = 2.5f;
        Vec2 size       = this->getContentSize();
        Vec2 sizeX      = size * Vec2::UNIT_X;
        Vec2 sizeY      = size * Vec2::UNIT_Y;

        Vec2 roundedCornerX = _roundedCorner * Vec2::UNIT_X;
        Vec2 roundedCornerY = _roundedCorner * Vec2::UNIT_Y;
        
        _drawNodeVisual->clear();
        _drawNodeVisual->drawLine(roundedCornerX, sizeX - roundedCornerX, Color4F::BLACK, lineThickness);
        _drawNodeVisual->drawLine(roundedCornerX + sizeY, size - roundedCornerX, Color4F::BLACK, lineThickness);
        _drawNodeVisual->drawLine(roundedCornerY, sizeY - roundedCornerY, Color4F::BLACK, lineThickness);
        _drawNodeVisual->drawLine(roundedCornerY + sizeX, size - roundedCornerY, Color4F::BLACK, lineThickness);

        _drawNodeVisual->drawQuadBezier(roundedCornerX, Vec2::ZERO, roundedCornerY, 10, Color4F::BLACK, curveThickness);
        _drawNodeVisual->drawQuadBezier(sizeY - roundedCornerY, sizeY, sizeY + roundedCornerX, 10, Color4F::BLACK, curveThickness);
        _drawNodeVisual->drawQuadBezier(size - roundedCornerX, size, size - roundedCornerY, 10, Color4F::BLACK, curveThickness);
        _drawNodeVisual->drawQuadBezier(sizeX + roundedCornerY, sizeX, sizeX - roundedCornerX, 10, Color4F::BLACK, curveThickness);

        //_drawNodeVisual->drawDot(this->getAnchorPointInPoints(), 5.f, ax::Color4F::RED);
    }, "placeholder_visual");
}
