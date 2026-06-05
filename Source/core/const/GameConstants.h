#pragma once

namespace ActionTag {
    static const int CARD_FLIP = 1001;
    static const int CARD_TRANSFORM_TO_ZONE = 2000;
    static const int CARD_PICKED_MOVE = 3000;
    }

namespace ObjectTag {
    static const int CARD = 10;
}

namespace ZOrder {
    static const int CARD_DEFAULT = 0;
    static const int CARD_DRAGGING = 100;
}
