#pragma once

#include "axmol.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"

#include <string>
#include <map>
#include <cstdio>



using namespace rapidjson;

class RapidJsonHelper
{
public:
    void test();
    void getDocumentFromFile(const std::string& filePath, Document& document);
    void getDocumentFromString(const std::string& jsonString, Document& document);
    void writeDocumentToFile(const std::string& filePath, const Document& document);
    std::string convertDocumentToString(const Document& document);
};
