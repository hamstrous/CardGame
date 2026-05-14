#include "UnoSetUpState.h"

#include "UnoDealState.h"

#include "utils/helper.h"

void UnoSetUpState::onEnter()
{
    // UnoState::onEnter();

    // bind reference to the game rule for easy access
    auto& game = *getContext();

    AXLOGD("Player index: {}", game._playerId);

    auto handPositionList = vector<Vec2>{
        Vec2(game.visibleSize.width / 2 + game.origin.x, game.visibleSize.height / 2 + game.origin.y - 300),  // bottom
        Vec2(game.visibleSize.width / 2 + game.origin.x - 400, game.visibleSize.height / 2 + game.origin.y),  // right
        Vec2(game.visibleSize.width / 2 + game.origin.x, game.visibleSize.height / 2 + game.origin.y + 300),  // top
        Vec2(game.visibleSize.width / 2 + game.origin.x + 400, game.visibleSize.height / 2 + game.origin.y)   // left
    };
    int listIndex = 0;
    ax::Vector<Zone*> tempPlayerHands;
    for (int i = game._playerId; i < game._playerCount; i++)
    {
        Zone* playerHand = Zone::create(new ZoneData());
        // play the zone in a circular layout around the center of the screen, the bottom player is the client player,
        // the others are opponents, going clockwise
        if (listIndex % 2 == 1)
            playerHand->setRotation(90);
        playerHand->setPosition(handPositionList[listIndex++]);
        playerHand->setContentSize(Size(300, 100));
        game.addChild(playerHand);
        tempPlayerHands.pushBack(playerHand);
        playerHand->lockInput();
    }
    for (int i = 0; i < game._playerId; i++)
    {
        Zone* playerHand = Zone::create(new ZoneData());
        if (listIndex % 2 == 1)
            playerHand->setRotation(90);
        playerHand->setPosition(handPositionList[listIndex++]);
        playerHand->setContentSize(Size(300, 100));
        game.addChild(playerHand);
        game._playerHands.pushBack(playerHand);
        playerHand->lockInput();
    }
    for (auto playerHand : tempPlayerHands)
    {
        // Make sure player contain hand from player index 0 -> n, not depend on player index
        game._playerHands.pushBack(playerHand);
    }

    // load all card files from content/card/uno

    game._deck = Deck::create(new ZoneData());
    game._deck->setPosition(150, game.visibleSize.height - 100);
    game._deck->setContentSize(Size(150, 100));
    game.addChild(game._deck);

    int cardIndex  = 0;
    auto cardFiles = helper::getFileNamesInFolder("card/uno");
    for (const auto& cardFile : cardFiles)
    {
        AXLOGD("Loading card file: {}", cardFile);
        Card* card = Card::create(new CardData("card/uno/" + cardFile, "card/Card Back 1.png"));
        game._cards.pushBack(card);
        card->setId(cardIndex++);
        card->setContentSize(ax::Size(100, 150));
        game.addChild(card);  // add to scene first to ensure the card can be move, and no problem with the remove parent
        game._deck->moveCardToThisZone(card);
    }

    game._discardPile = Zone::create(new ZoneData());
    game._discardPile->setPosition(game.visibleSize / 2);
    game._discardPile->setContentSize(Size(150, 150));
    game.addChild(game._discardPile);

    game.changeState(new UnoDealState(getContext()));
}

void UnoSetUpState::onUpdate(float delta) {}

void UnoSetUpState::onExit() {}

void UnoSetUpState::onMouseDown(ax::Event* event) {}

void UnoSetUpState::onMouseUp(ax::Event* event) {}

void UnoSetUpState::onMouseMove(ax::Event* event) {}

void UnoSetUpState::onMouseScroll(ax::Event* event) {}

void UnoSetUpState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoSetUpState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void UnoSetUpState::onWebSocketMessage(EventWebSocket* event) {}
