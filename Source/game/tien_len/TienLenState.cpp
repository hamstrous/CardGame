#include "TienLenState.h"

TienLenRule::Rank TienLenState::convertStringToTienLenRank(const std::string& str)
{
    if (str == "1")
        return TienLenRule::Rank::ACE;
    else if (str == "2")
        return TienLenRule::Rank::TWO;
    else if (str == "3")
        return TienLenRule::Rank::THREE;
    else if (str == "4")
        return TienLenRule::Rank::FOUR;
    else if (str == "5")
        return TienLenRule::Rank::FIVE;
    else if (str == "6")
        return TienLenRule::Rank::SIX;
    else if (str == "7")
        return TienLenRule::Rank::SEVEN;
    else if (str == "8")
        return TienLenRule::Rank::EIGHT;
    else if (str == "9")
        return TienLenRule::Rank::NINE;
    else if (str == "10")
        return TienLenRule::Rank::TEN;
    else if (str == "11")
        return TienLenRule::Rank::JACK;
    else if (str == "12")
        return TienLenRule::Rank::QUEEN;
    else if (str == "13")
        return TienLenRule::Rank::KING;
    AXLOGE("Invalid TienLen rank string: {}", str);
    return TienLenRule::Rank();
}

TienLenRule::Suit TienLenState::convertStringToTienLenSuit(const std::string& str)
{
    if (str == "Diamonds")
        return TienLenRule::Suit::DIAMONDS;
    else if (str == "Clubs")
        return TienLenRule::Suit::CLUBS;
    else if (str == "Hearts")
        return TienLenRule::Suit::HEARTS;
    else if (str == "Spades")
        return TienLenRule::Suit::SPADES;
    AXLOGE("Invalid TienLen suit string: {}", str);
    return TienLenRule::Suit();
}
