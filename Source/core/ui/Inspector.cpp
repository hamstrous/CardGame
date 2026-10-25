#include "Inspector.h"

Inspector* Inspector::_instance = nullptr;

Inspector* Inspector::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new Inspector();
    }
    return _instance;
}

void Inspector::destroyInstance()
{
    AX_SAFE_DELETE(_instance);
}

void Inspector::draw() {
    ImGui::Begin("Inspector");

    ImGui::Button("Button");
    static int i1 = 50;
    ImGui::DragInt("drag int", &i1, 1);

    ImGui::End();
}
