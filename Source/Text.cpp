#include "Text.h"
using namespace ax;

const ax::Vec2 Text::TEXT_SIZE   = ax::Vec2(200.0f, 50.0f);
const float Text::TEXT_FONT_SIZE = 24.0f;

Text* Text::create()
{
    Text* text = new (std::nothrow) Text();
    if (text && text->init())
    {
        text->autorelease();
        return text;
    }
    AX_SAFE_DELETE(text);
    return nullptr;
}

bool Text::init()
{
    if (!Node::init())
        return false;

    _objectSize  = TEXT_SIZE;
    _isDraggable = true;

    // Create label with default "Text"
    _textLabel = ax::Label::createWithSystemFont("Text", "Arial", TEXT_FONT_SIZE);
    if (!_textLabel)
    {
        AXLOG("Failed to create text label");
        return false;
    }

    _textLabel->setAnchorPoint(ax::Vec2(0.5f, 0.5f));
    _textLabel->setPosition(ax::Vec2(0, 0));
    _textLabel->setTextColor(ax::Color4B::BLACK);
    this->addChild(_textLabel);

    setContentSize(TEXT_SIZE);
    _currentText = "Text";

    return true;
}

Text* Text::clone() const
{
    Text* newText = Text::create();
    if (newText)
    {
        newText->setText(_currentText);
        newText->setPosition(this->getPosition());
    }
    return newText;
}

void Text::startEditing()
{
    if (_isEditing)
        return;

    _isEditing = true;

    // Visual feedback for editing mode - add highlight and change text color
    setHighlight(true);
    if (_textLabel)
    {
        _textLabel->setTextColor(ax::Color4B::BLUE);
    }
}

void Text::stopEditing()
{
    if (!_isEditing)
        return;

    _isEditing = false;

    // Remove highlight and restore normal color
    setHighlight(false);
    if (_textLabel)
    {
        _textLabel->setTextColor(ax::Color4B::BLACK);
    }

    // If text is empty, set default "Text"
    if (_currentText.empty())
    {
        _currentText = "Text";
        updateLabel();
    }
}

void Text::setText(const std::string& text)
{
    if (text.empty())
    {
        _currentText = "Text";
    }
    else
    {
        _currentText = text;
    }
    updateLabel();

    // Update object size based on text dimensions
    if (_textLabel)
    {
        ax::Size labelSize = _textLabel->getContentSize();
        // Add padding around the text
        float padding = 10.0f;
        _objectSize   = ax::Vec2(labelSize.width + padding * 2, labelSize.height + padding * 2);
        setContentSize(_objectSize);
    }
}

std::string Text::getText() const
{
    return _currentText;
}

void Text::updateLabel()
{
    if (_textLabel)
    {
        _textLabel->setString(_currentText);

        // Update object size based on text dimensions
        ax::Size labelSize = _textLabel->getContentSize();
        // Add padding around the text
        float padding = 10.0f;
        _objectSize   = ax::Vec2(labelSize.width + padding * 2, labelSize.height + padding * 2);
        setContentSize(_objectSize);
    }
}

// void Text::setHighlight(bool highlight)
// {
//     // Remove old highlight if exists
//     if (_highlightNode)
//     {
//         this->removeChild(_highlightNode);
//         _highlightNode = nullptr;
//         this->setScale(_originalScale);
//     }

//     // Add new highlight with current size
//     if (highlight)
//     {
//         _highlightNode = ax::DrawNode::create();
//         Vec2 rect[4];
//         float w = _objectSize.x;
//         float h = _objectSize.y;
//         rect[0] = Vec2(-w / 2, -h / 2);
//         rect[1] = Vec2(w / 2, -h / 2);
//         rect[2] = Vec2(w / 2, h / 2);
//         rect[3] = Vec2(-w / 2, h / 2);
//         _highlightNode->drawPoly(rect, 4, true, Color4F::YELLOW);
//         this->addChild(_highlightNode, 100);
//         this->setScale(_originalScale * 1.05f);
//     }

//     // Also change text color to blue when highlighted during editing
//     if (_textLabel && _isEditing)
//     {
//         if (highlight)
//         {
//             _textLabel->setTextColor(ax::Color4B::BLUE);
//         }
//         else
//         {
//             _textLabel->setTextColor(ax::Color4B::BLACK);
//         }
//     }
// }
