#pragma once

#include <vector>
#include <string>

static std::vector<std::string> split(const std::string& str, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    std::string token;
    for (char ch : str)
    {
        if (ch == delimiter)
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else
        {
            token += ch;
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

static std::string getTextFileContent(const std::string& filePath)
{
    auto fileUtils = ax::FileUtils::getInstance();
    std::string content = fileUtils->getStringFromFile(filePath);
    return content;
}
