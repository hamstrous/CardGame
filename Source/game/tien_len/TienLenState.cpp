#include "TienLenState.h"

TienLenRule::Value TienLenState::convertStringToTienLenValue(const std::string& str)
{
    if (str == "0")
        return TienLenRule::Value::ZERO;
    else if (str == "1")
        return TienLenRule::Value::ONE;
    else if (str == "2")
        return TienLenRule::Value::TWO;
    else if (str == "3")
        return TienLenRule::Value::THREE;
    else if (str == "4")
        return TienLenRule::Value::FOUR;
    else if (str == "5")
        return TienLenRule::Value::FIVE;
    else if (str == "6")
        return TienLenRule::Value::SIX;
    else if (str == "7")
        return TienLenRule::Value::SEVEN;
    else if (str == "8")
        return TienLenRule::Value::EIGHT;
    else if (str == "9")
        return TienLenRule::Value::NINE;
    else if (str == "skip")
        return TienLenRule::Value::SKIP;
    else if (str == "inverse")
        return TienLenRule::Value::REVERSE;
    else if (str == "2plus")
        return TienLenRule::Value::DRAW_TWO;
    else if (str == "wild")
        return TienLenRule::Value::WILD;
    else if (str == "4plus")
        return TienLenRule::Value::WILD_DRAW_FOUR;
    AXLOGE("Invalid UNO value string: {}", str);
    return TienLenRule::Value();
}

TienLenRule::Color TienLenState::convertStringToTienLenColor(const std::string& str)
{
    if (str == "red")
        return TienLenRule::Color::RED;
    else if (str == "blue")
        return TienLenRule::Color::BLUE;
    else if (str == "green")
        return TienLenRule::Color::GREEN;
    else if (str == "yellow")
        return TienLenRule::Color::YELLOW;
    else if (str == "wild")
        return TienLenRule::Color::WILD;  // default to WILD if color is not recognized
    AXLOGE("Invalid UNO color string: {}", str);
    return TienLenRule::Color();
}

bool TienLenState::isCardSpecial(Card* card)
{
    TienLenRule::Value cardValue = static_cast<TienLenRule::Value>(card->getValue("value"));
    TienLenRule::Color cardColor = static_cast<TienLenRule::Color>(card->getValue("color"));
    if (cardValue >= TienLenRule::Value::SKIP || cardColor == TienLenRule::Color::WILD)
    {
        return true;
    }
    return false;
}
