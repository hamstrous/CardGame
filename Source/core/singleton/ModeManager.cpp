#include "ModeManager.h"

using namespace ax;
using namespace std;

ModeManager* ModeManager::_instance = nullptr;

ModeManager::ModeManager()
{
    _keyboardListener = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed = AX_CALLBACK_2(ModeManager::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, -1);
}

ModeManager* ModeManager::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ModeManager();
    }
    return _instance;
}

void ModeManager::destroyInstance()
{
    if (_instance)
    {
        _instance->removeFromParentAndCleanup(true);
        AX_SAFE_DELETE(_instance);
    }   
}

void ModeManager::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    EventMode* e = nullptr;
    if (code == EventKeyboard::KeyCode::KEY_E)
    {
        e = new EventMode(Mode::EDIT);
    }
    else if (code == EventKeyboard::KeyCode::KEY_P)
    {
        e = new EventMode(Mode::PLAY);
    }
    else if (code == EventKeyboard::KeyCode::KEY_S)
    {
        e = new EventMode(Mode::SELECT);
    }
    else
    {
        return;
    }
    currentMode = e->getMode();
    _eventDispatcher->dispatchEvent(e);
}
