#pragma once

#include "axmol.h"

#include "ImGui/ImGuiPresenter.h"
USING_NS_AX_EXT;
using namespace std;

class InspectorDataItem
{

public:
    class Item {
    public:
        Item(string name, float* ref, function<void(float)> callback) : name(name), ref(ref), callback(callback) {};
        string name;
        float* ref;
        function<void(float)> callback;
    };

    enum Type {
        DRAG_FLOAT,
        SLIDER_FLOAT,
        SELECTABLE_BUTTON
    };


    InspectorDataItem(string name, string description, Type type) : name(name), description(description), type(type) {};

    void Add(Item item) { list.push_back(item); }

    //virtual void drawGuiItem();

    string name;
    string description;
    Type type;
    vector<Item> list;

};
