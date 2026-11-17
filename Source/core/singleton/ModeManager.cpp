#include "ModeManager.h"

using namespace ax;
using namespace std;

ModeManager* ModeManager::_instance = nullptr;

ModeManager::ModeManager()
{
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
    EventMode* e;
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
    currentMode = e->getMode();
    _eventDispatcher->dispatchEvent(e);
}
