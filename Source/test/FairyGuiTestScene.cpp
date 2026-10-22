#include "FairyGuiTestScene.h"

using namespace ax;

bool FairyGuiTestScene::init()
{
    // super init
    if (!Scene::init())
    {
        return false;
    }

    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _groot = GRoot::create(this);
    _groot->retain();

    // add a closebutton to scene
    GObject* closeButton = UIPackage::createObject("MainMenu", "CloseButton");
    closeButton->setPosition(_groot->getWidth() - closeButton->getWidth() - 10,
                             _groot->getHeight() - closeButton->getHeight() - 10);
    closeButton->addRelation(_groot, RelationType::Right_Right);
    closeButton->addRelation(_groot, RelationType::Bottom_Bottom);
    closeButton->setSortingOrder(100000);
    closeButton->addClickListener(AX_CALLBACK_1(FairyGuiTestScene::onClose, this));
    _groot->addChild(closeButton);

    return true;
}

void FairyGuiTestScene::update(float delta) {}


void FairyGuiTestScene::onClose(EventContext* context)
{
    AXLOGD("Close button clicked, exiting the application...");
    _director->end();
}

void FairyGuiTestScene::menuCloseCallback(ax::Object* sender) {}

FairyGuiTestScene::FairyGuiTestScene() {}

FairyGuiTestScene::~FairyGuiTestScene()
{
    AX_SAFE_RELEASE(_groot);
}
