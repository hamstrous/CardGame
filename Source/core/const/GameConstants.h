#pragma once

namespace constant
{

enum
{
    CARD_FLIP              = 1001,
    CARD_TRANSFORM_TO_ZONE = 2000,
    CARD_PICKED_MOVE       = 3000
};

enum ObjectTag
{
    CARD = 10
};

enum ZOrder
{
    CARD_DEFAULT  = 0,
    CARD_DRAGGING = 100
};

enum ActionTag
{
    WIDGET_ROTATION
};

enum Mode
{
    EDIT,
    PLAY,
    SELECT
};

}  // namespace constant
