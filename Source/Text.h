#pragma once

#include "axmol.h"
#include "DraggableObject.h"

class Text : public DraggableObject
{
public:
    static Text* create();

    virtual bool init();
    Text* clone() const;

    void startEditing();
    void stopEditing();
    bool isEditing() const { return _isEditing; }

    void setText(const std::string& text);
    std::string getText() const;

    //virtual void setHighlight(bool highlight) override;

    static const ax::Vec2 TEXT_SIZE;
    static const float TEXT_FONT_SIZE;

private:
    ax::Label* _textLabel    = nullptr;
    bool _isEditing          = false;
    std::string _currentText = "";

    void setupTextField();
    void updateLabel();
};
