#include "UnoState.h"

UnoRule::Value UnoState::convertStringToUnoValue(const std::string& str)
{
    if (str == "0")
        return UnoRule::Value::ZERO;
    else if (str == "1")
        return UnoRule::Value::ONE;
    else if (str == "2")
        return UnoRule::Value::TWO;
    else if (str == "3")
        return UnoRule::Value::THREE;
    else if (str == "4")
        return UnoRule::Value::FOUR;
    else if (str == "5")
        return UnoRule::Value::FIVE;
    else if (str == "6")
        return UnoRule::Value::SIX;
    else if (str == "7")
        return UnoRule::Value::SEVEN;
    else if (str == "8")
        return UnoRule::Value::EIGHT;
    else if (str == "9")
        return UnoRule::Value::NINE;
    else if (str == "skip")
        return UnoRule::Value::SKIP;
    else if (str == "inverse")
        return UnoRule::Value::REVERSE;
    else if (str == "2plus")
        return UnoRule::Value::DRAW_TWO;
    else if (str == "wild")
        return UnoRule::Value::WILD;
    else if (str == "4plus")
        return UnoRule::Value::WILD_DRAW_FOUR;
    AXLOGE("Invalid UNO value string: {}", str);
    return UnoRule::Value();
}

UnoRule::Color UnoState::convertStringToUnoColor(const std::string& str)
{
    if (str == "red")
        return UnoRule::Color::RED;
    else if (str == "blue")
        return UnoRule::Color::BLUE;
    else if (str == "green")
        return UnoRule::Color::GREEN;
    else if (str == "yellow")
        return UnoRule::Color::YELLOW;
    else if (str == "wild")
        return UnoRule::Color::WILD;  // default to WILD if color is not recognized
    AXLOGE("Invalid UNO color string: {}", str);
    return UnoRule::Color();
}

bool UnoState::isCardSpecial(Card* card)
{
    UnoRule::Value cardValue = static_cast<UnoRule::Value>(card->getValue("value"));
    UnoRule::Color cardColor = static_cast<UnoRule::Color>(card->getValue("color"));
    if (cardValue >= UnoRule::Value::SKIP || cardColor == UnoRule::Color::WILD)
    {
        return true;
    }
    return false;
}
