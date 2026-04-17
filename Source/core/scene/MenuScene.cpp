#include "MenuScene.h"

using namespace ax;

static int s_sceneID = 1001;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    // super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    // Some templates (uncomment what you  need)

    _mouseListener                = EventListenerMouse::create();
    _mouseListener->onMouseMove   = AX_CALLBACK_1(MenuScene::onMouseMove, this);
    _mouseListener->onMouseUp     = AX_CALLBACK_1(MenuScene::onMouseUp, this);
    _mouseListener->onMouseDown   = AX_CALLBACK_1(MenuScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(MenuScene::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(MenuScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    processStartFile();
    setUpButtonsPanel();

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

bool MenuScene::onMouseDown(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseDown detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool MenuScene::onMouseUp(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    //AXLOGD("onMouseUp detected, button: {}", static_cast<int>(e->getMouseButton()));
    return true;
}

bool MenuScene::onMouseMove(Event* event)
{
    EventMouse* e = static_cast<EventMouse*>(event);
    // AXLOGD("onMouseMove detected, X:{}  Y:{}", e->getLocation().x, e->getLocation().y);
    return true;
}

void MenuScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
}

void MenuScene::update(float delta)
{
    switch (_gameState)
    {
    case GameState::init:
    {
        _gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    }  // switch
}

void MenuScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

}

void MenuScene::processStartFile() {
    //std::string content = getTextFileContent(START_FILE);
    //AXLOG(content.c_str());

    //std::istringstream stream(content);

    //std::string gameTitle;
    //while (stream >> gameTitle)
    //{
    //    MenuButton* button = MenuButton::create("icon/" + gameTitle + ".png", gameTitle);
    //    if (button)
    //    {
    //        //button->setScale9Enabled(true);
    //        button->setContentSize(Size(100, 50));
    //        _menuButtonList.pushBack(button);
    //    }
    //}
    MenuButton* button = MenuButton::create<GameScene>("icon/uno.png", "uno");
        if (button)
        {
            button->setContentSize(Size(100, 50));
            _menuButtonList.pushBack(button);
        }
}

void MenuScene::setUpButtonsPanel() {
    int size = _menuButtonList.size();
    const int rowMax = 9;

    for (int i = 0; i < size; ++i)
    {
        MenuButton* button = _menuButtonList.at(i);
        if (button)
        {
            this->addChild(button);
            int row = i / rowMax;
            int col = i % rowMax;
            button->setPosition(Vec2(100 + col * 110, 400 - row * 60));
            button->setContentSize(Size(1000, 50));
        }
    }
}

MenuScene::MenuScene()
{
    _sceneID = ++s_sceneID;
    AXLOGD("Scene: ctor: #{}", _sceneID);
}

MenuScene::~MenuScene()
{
    AXLOGD("~Scene: dtor: #{}", _sceneID);

    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
    _sceneID = -1;
}
