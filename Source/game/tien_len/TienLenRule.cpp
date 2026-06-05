#include "TienLenRule.h"
#include "TienLenSetUpState.h"
#include "TienLenResultState.h"
#include "TienLenPlayState.h"

#include "core/scene/GameScene.h"

#include "utils/magic_enum.hpp"

bool TienLenRule::init()
{
    visibleSize = _director->getVisibleSize();
    origin      = _director->getVisibleOrigin();
    safeArea    = _director->getSafeAreaRect();
    safeOrigin  = safeArea.origin;

    _mouseListener              = EventListenerMouse::create();
    _mouseListener->onMouseMove = AX_CALLBACK_1(TienLenRule::onMouseMove, this);
    _mouseListener->onMouseUp   = AX_CALLBACK_1(TienLenRule::onMouseUp, this);
    _mouseListener->onMouseDown = AX_CALLBACK_1(TienLenRule::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);

    _keyboardListener                = EventListenerKeyboard::create();
    _keyboardListener->onKeyPressed  = AX_CALLBACK_2(TienLenRule::onKeyPressed, this);
    _keyboardListener->onKeyReleased = AX_CALLBACK_2(TienLenRule::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_keyboardListener, 11);

    _websocketListener                     = EventListenerWebSocket::create();
    _websocketListener->onWebSocketMessage = AX_CALLBACK_1(TienLenRule::onWebSocketMessage, this);
    _eventDispatcher->addEventListenerWithFixedPriority(_websocketListener, 1);

    _cardListener = EventListenerCard::create();
    _cardListener->onCardClicked = AX_CALLBACK_1(TienLenRule::onCardClicked, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_cardListener, this);

    _socketManager = SocketNetworkManager::getInstance();

    _gameInfo = ax::ui::Text::create("Name: \nOrder: \nCurrent turn: \nLast played card: ", "Arial", 20);
    _gameInfo->setPositionX(origin.x + visibleSize.width - _gameInfo->getContentSize().width);
    _gameInfo->setPositionY(origin.y + _gameInfo->getContentSize().height/2);
    this->addChild(_gameInfo);

    scheduleUpdate();

    changeState(new TienLenSetUpState(this));

    return true;
}

void TienLenRule::update(float delta) {
    
    if (dynamic_cast<TienLenPlayState*>(_currentState))
    {
        // update game info
        std::string orderText    = "Order: " + std::string(_clockWise ? "Clockwise" : "Counter-clockwise");
        std::string turnText     = "Current turn: Player " + std::to_string(_currentPlayerId);
        _gameInfo->setString(std::string("Name: ") + GameScene::getInstance()->getUserName() + "\n" + orderText + "\n" +
                             turnText);

        for (int i = 0; i < _playerHands.size(); i++)
        {
            auto zone        = _playerHands[i];
            auto cardsInHand = helper::castToVectorOfType<Card*>(zone->getChildren());
            if (cardsInHand.empty())
            {
                if (i == _clientPlayerId)
                {
                    changeState(new TienLenResultState(this, "You win!"));
                }
                else
                {
                    changeState(new TienLenResultState(this, "You lose!"));
                }
                break;
            }
        }
    }
    Rule::update(delta);
}

void TienLenRule::onCardClicked(EventCard* event) {
    if (_currentState)
    {
        static_cast<TienLenState*>(_currentState)->onCardClicked(event);
    }
}

bool TienLenRule::isWinHand(const ax::Vector<Card*>& hand)
{
    return false;
}

bool TienLenRule::isValidPlay(const std::set<Card*>& hand)
{
    vector<int> handValues;
    vector<int> rankCount(13, 0);
    int previousRank     = -1;
    int consecutiveCount = 0;
    int maxSameRankCount = 0;
    int minSameRankCount = INT_MAX;
    for (auto card : hand)
    {
        handValues.push_back(convertTienLenCardToValue(card));
    }

    sort(handValues.begin(), handValues.end());
    int rank = -1;
    for (int value : handValues)
    {
        rank = value / 4;  // Get the rank of the card
        rankCount[rank]++;
        if (rank != previousRank)
        {
            consecutiveCount++;
            maxSameRankCount = max(maxSameRankCount, rankCount[rank]);
            minSameRankCount = min(minSameRankCount, rankCount[rank]);
            previousRank     = rank;
        }
    }
    consecutiveCount++;
    maxSameRankCount = max(maxSameRankCount, rankCount[rank]);
    minSameRankCount = min(minSameRankCount, rankCount[rank]);
    previousRank     = rank;

    // Valid combination
    if (!((consecutiveCount == 1 || consecutiveCount > 2) && maxSameRankCount == minSameRankCount))
        return false;

    int sameRankCount = maxSameRankCount;

   
    Combination combination =
        (consecutiveCount == 1)
            ? (maxSameRankCount == 1 ? Combination::SINGLE
                                     : (maxSameRankCount == 2 ? Combination::PAIR
                                                              : (maxSameRankCount == 3 ? Combination::TRIPLE
                                                                                       : Combination::FOUR_OF_A_KIND)))
            : (maxSameRankCount == 2
                   ? (consecutiveCount == 3 ? Combination::THREE_DOUBLE_SEQUENCE
                                            : (maxSameRankCount == 4 ? Combination::FOUR_DOUBLE_SEQUENCE
                                                                     : Combination::STRAIGHT))
                   : Combination::SINGLE);  // Default to SINGLE if it doesn't fit other combinations

    bool endWithTwo = handValues.back() / 4 == static_cast<int>(Rank::TWO);

    bool valid = false;

     if (_isNewRound)
     {
        valid = true;
     }


    if (_currentCombination == combination && handValues.back() >= _currentLargestCardValue)
        valid = true;
    else if (_currentCombination != combination)
    {
        if (_currentCombination == Combination::SINGLE && _currentEndWithTwo)
            valid |= static_cast<int>(combination) > static_cast<int>(Combination::THREE_DOUBLE_SEQUENCE);
        else if (_currentCombination == Combination::PAIR && _currentEndWithTwo)
            valid |= combination == Combination::FOUR_DOUBLE_SEQUENCE;
        else if (_currentCombination >= Combination::THREE_DOUBLE_SEQUENCE)
            valid |= combination > _currentCombination;
    }

    if (!valid)
        return false;

    _previousPlayedCards.clear();
    for (int value : handValues)
    {
        _previousPlayedCards.push_back(value);
    }
    _currentConsecutiveCount = consecutiveCount;
    _currentSameRankCount    = sameRankCount;
    _currentLargestCardValue = handValues.back();
    _currentEndWithTwo       = endWithTwo;
    _currentCombination      = combination;
    return true;
}

int TienLenRule::convertTienLenCardToValue(Card* card)
{
    return card->getValue("rank") * 4 + card->getValue("suit");
}
