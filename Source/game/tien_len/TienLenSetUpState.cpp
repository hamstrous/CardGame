#include "TienLenSetUpState.h"

#include "TienLenDealState.h"

#include "utils/helper.h"

void TienLenSetUpState::onEnter()
{
    // TienLenState::onEnter();

    // bind reference to the game rule for easy access
    auto& game = *getContext();

    auto handPositionList = vector<Vec2>{
        Vec2(game.visibleSize.width / 2 + game.origin.x, game.visibleSize.height / 2 + game.origin.y - 300),  // bottom
        Vec2(game.visibleSize.width / 2 + game.origin.x - 400, game.visibleSize.height / 2 + game.origin.y),  // right
        Vec2(game.visibleSize.width / 2 + game.origin.x, game.visibleSize.height / 2 + game.origin.y + 300),  // top
        Vec2(game.visibleSize.width / 2 + game.origin.x + 400, game.visibleSize.height / 2 + game.origin.y)   // left
    };
    int listIndex = 0;
    ax::Vector<Zone*> tempPlayerHands;
    for (int i = game._clientPlayerId; i < game._playerCount; i++)
    {
        Zone* playerHand = Zone::create(new ZoneData());
        // play the zone in a circular layout around the center of the screen, the bottom player is the client player,
        // the others are opponents, going clockwise
        if (listIndex % 2 == 1)
            playerHand->setRotation(90);
        playerHand->setPosition(handPositionList[listIndex++]);
        if (i == game._clientPlayerId)
        {
            playerHand->setContentSize(Size(850, 100));
        }
        else
        {
            playerHand->setContentSize(Size(300, 100));
        }
        game.addChild(playerHand);
        tempPlayerHands.pushBack(playerHand);
        playerHand->lockInput();
    }
    for (int i = 0; i < game._clientPlayerId; i++)
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

    // load all card files from content/card/tien_len

    game._deck = Deck::create(new ZoneData());
    game._deck->setPosition(150, game.visibleSize.height - 120);
    game._deck->setContentSize(Size(130, 150));
    game._deck->lockInput();
    game.addChild(game._deck);

    int cardIndex  = 0;
    auto cardFiles = helper::getFileNamesInFolder("card/standard");
    for (const auto& cardFile : cardFiles)
    {
        //AXLOGD("Loading card file: {}", cardFile);

        auto cardFileName = helper::split(cardFile, '.')[0];  // remove file extension
        auto cardInfo = helper::split(cardFileName, ' ');

        Card* card = Card::create(new CardData("card/standard/" + cardFile, "card/Card Back 1.png"));

        if (cardInfo[0] == "Empty")
        {
            // This is a placeholder card, skip it
            continue;
        }

        card->setValue("suit", static_cast<int>(convertStringToTienLenSuit(cardInfo[0])));
        card->setValue("rank", static_cast<int>(convertStringToTienLenRank(cardInfo[1])));
        card->forceHide();
        card->lockInput();
        card->setDraggable(false);
        card->setFlippable(false);
        game._cards.pushBack(card);
        card->setId(cardIndex++);
        card->setContentSize(ax::Size(100, 150));
        game.addChild(card);  // add to scene first to ensure the card can be move, and no problem with the remove parent
        game._deck->moveCardToThisZone(card);
    }

    game._discardPile = Zone::create(new ZoneData());
    game._discardPile->setPosition(game.visibleSize / 2);
    game._discardPile->setContentSize(Size(300, 150));
    game._discardPile->lockInput();
    game.addChild(game._discardPile);

    // set up play and pass button
    game._playButton = ax::ui::Button::create("ui/button.png");
    game._passButton = ax::ui::Button::create("ui/button.png");
    game._playButton->ignoreContentAdaptWithSize(false);
    game._passButton->ignoreContentAdaptWithSize(false);
    game._playButton->setPosition(Vec2(game.visibleSize.width / 2 - 100, game.visibleSize.height / 2 - 150));
    game._passButton->setPosition(Vec2(game.visibleSize.width / 2 + 100, game.visibleSize.height / 2 - 150));
    game._playButton->setContentSize(Size(120, 60));
    game._passButton->setContentSize(Size(120, 60));
    game._playButton->setTitleText("Play");
    game._passButton->setTitleText("Pass");
    game._playButton->setTitleFontSize(24);
    game._passButton->setTitleFontSize(24);
    game._playButton->setVisible(false);
    game._passButton->setVisible(false);

    game.addChild(game._playButton);
    game.addChild(game._passButton);

    game.changeState(new TienLenDealState(getContext()));

}

void TienLenSetUpState::onUpdate(float delta) {
    auto& game = *getContext();
    for (auto card : helper::castToVectorOfType<Card*>(game._playerHands[game._clientPlayerId]->getChildren()))
    {
        card->forceReveal();
    }
}

void TienLenSetUpState::onExit() {}

void TienLenSetUpState::onMouseDown(ax::Event* event) {}

void TienLenSetUpState::onMouseUp(ax::Event* event) {}

void TienLenSetUpState::onMouseMove(ax::Event* event) {}

void TienLenSetUpState::onMouseScroll(ax::Event* event) {}

void TienLenSetUpState::onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenSetUpState::onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) {}

void TienLenSetUpState::onWebSocketMessage(EventWebSocket* event) {}
