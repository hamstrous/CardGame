#pragma once

#include "axmol.h"

#include "core/event/EventListenerWebsocket.h"
#include "core/event/EventListenerCard.h"
#include "core/object/Card.h"
#include "core/object/Zone.h"
#include "core/object/Deck.h"

#include "ui/UIButton.h"

#include "core/event/EventCard.h"

#include "ui/UIText.h"

#include "core/rule/Rule.h"

class TestRule : public Rule
{
public:

public:

    bool init() override;
    void update(float delta) override;  

protected:

    // A game rule class will let all states access ít private members
    friend class TestState;
};
